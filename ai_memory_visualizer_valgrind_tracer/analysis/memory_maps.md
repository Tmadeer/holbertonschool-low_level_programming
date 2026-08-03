# Memory Maps — AI Memory Visualizer

## 1. Introduction

This document analyzes runtime memory behavior in `stack_example.c` and
`aliasing_example.c`, using real addresses obtained by compiling and running
the programs directly (not simulated). AI tools were used to generate an
initial memory explanation, which was then critically reviewed against actual
program output. Several inaccuracies were found and are documented in
Section 4.

Environment: WSL (Ubuntu), `gcc` with flags from the provided Makefile
(`-Wall -Wextra -Werror -pedantic -std=gnu89 -g`).

---

## 2. `stack_example.c` — Stack Memory Analysis

### 2.1 Program behavior

`main()` calls `walk_stack(0, 3)`, which recurses from `depth=0` to
`depth=3`. At each depth, `walk_stack()` declares a local `int marker` and
calls `dump_frame()`, which declares its own `local_int`, `local_buf[16]`,
and a pointer `p_local` pointing at `local_int`.

### 2.2 Observed addresses (real run)

| depth | &local_int   | &marker      |
|-------|--------------|--------------|
| 0     | 0x7ffdfd96a6d4 | 0x7ffdfd96a724 |
| 1     | 0x7ffdfd96a6a4 | 0x7ffdfd96a6f4 |
| 2     | 0x7ffdfd96a674 | 0x7ffdfd96a6c4 |
| 3     | 0x7ffdfd96a644 | 0x7ffdfd96a694 |

### 2.3 Stack memory classification

All variables in this program — `local_int`, `local_buf`, `p_local` (in
`dump_frame`), and `marker` (in `walk_stack`) — are **stack memory**. They
are allocated automatically when their enclosing function is entered
(as part of that call's stack frame) and are deallocated automatically
when the function returns. No `malloc`/`free` is involved anywhere in this
file.

### 2.4 Stack growth direction

Each recursive call produces a **smaller** address for `local_int`:
`0x...a6d4 → 0x...a6a4 → 0x...a674 → 0x...a644`, a constant decrease of
`0x30` (48 bytes) per level. This confirms that on this platform
(x86-64 Linux), **the stack grows toward lower addresses** as new frames
are pushed.

### 2.5 Variable lifetimes

- `local_int`, `local_buf`, `p_local` in `dump_frame(depth=N)`: lifetime
  begins when `dump_frame` is entered for that specific call, and ends the
  instant that specific call returns. Each depth has its **own independent
  copy** — they are not shared across recursion levels, even though they
  share the same source-code name.
- `marker` in `walk_stack(depth=N)`: same rule — scoped strictly to that
  one invocation.
- Proof of reuse: `depth=3 [enter]` and `depth=3 [exit]` report the
  **exact same address** (`0x7ffdfd96a644`) for `local_int`. This shows the
  frame is not a "new" region each time in an absolute sense — it is the
  *same physical stack slot*, reused for the *same call*, and becomes
  invalid (available for the next push) the moment that call returns.

### 2.6 Pointer aliasing within `dump_frame`

`p_local = &local_int;` creates an alias: `p_local` and `&local_int` are
numerically identical (`p_local=0x...a6d4` == `&local_int=0x...a6d4` at
depth 0). This is a **safe** alias because `p_local`'s lifetime does not
outlive `local_int`'s lifetime — both belong to the same stack frame and
both become invalid at the same `return`. No dangling occurs in this
program because `p_local` is never read or stored anywhere after
`dump_frame` returns.

---

## 3. `aliasing_example.c` — Heap Aliasing & Use-After-Free

### 3.1 Program behavior

`make_numbers(5)` allocates a 5-element `int` array on the heap and
returns its address, stored in `a`. Then `b = a;` is executed. `free(a)`
is called, followed by reads and a write through `b`.

### 3.2 Observed output (real run)

```
a=0x62798e7426b0 b=0x62798e7426b0 a[2]=22 b[2]=22
after free(a): b=0x62798e7426b0 (dangling)
reading b[2]=-646236049
wrote b[3]=1234
```

### 3.3 Heap memory classification

The array returned by `make_numbers()` lives on the **heap** — it was
created with `malloc()` inside that function and its lifetime is **not**
tied to any function's scope. It remains valid until explicitly `free()`d,
regardless of which function is currently executing.

### 3.4 Pointer aliasing

After `b = a;`, both `a` and `b` hold the identical address
`0x62798e7426b0`. This is **pointer aliasing**: two independently-named
variables referring to the same single heap allocation. No new memory was
created; `b` is not a copy of the array, only a copy of the pointer value.

### 3.5 Ownership and the use-after-free

`free(a)` releases the heap block back to the allocator. However, `b`
still holds the same address it had before — `free()` does not, and
cannot, update every alias that happens to point at that block. `b` is
now a **dangling pointer**: syntactically valid (it holds a real-looking
address) but semantically invalid (the memory it refers to is no longer
owned by the program).

- `reading b[2]` after the free returns `-646236049` — not the original
  `22`. This demonstrates that `free()` does **not** clear or zero the
  block's contents; the observed value is simply whatever bytes are
  present at that address, which is unpredictable and must never be
  relied upon (classic **use-after-free read**).
- `b[3] = 1234;` is a **use-after-free write**. The program did not
  crash and printed `wrote b[3]=1234` — proving that writing to freed
  memory is undefined behavior, not a guaranteed crash. It can silently
  corrupt the heap if that block gets reallocated for something else by
  a later `malloc()` call, which is exactly the class of bug Valgrind is
  designed to catch even when no visible symptom appears.

---

## 4. `heap_example.c` — Heap Allocation & a Deliberate Leak

### 4.1 Program behavior

`person_new(name, age)` performs **two separate heap allocations** per
`Person`:

1. `p = malloc(sizeof(Person))` — the struct itself.
2. `p->name = malloc(len + 1)` — a second, independent block for the
   name string, referenced *from inside* the first block.

`main()` creates two people, `alice` and `bob`, each producing this pair
of allocations. It then frees them using two different (and
inconsistent) code paths.

### 4.2 Observed addresses (real run)

```
alice=0x5710eebf76b0 name=0x5710eebf76d0 age=30
bob=0x5710eebf76f0   name=0x5710eebf7710 age=41
```

Each `Person` struct address and its `name` address are distinct,
confirming these are **two separate `malloc` calls**, not one combined
allocation — freeing the struct does **not** automatically free the
string it points to.

### 4.3 Heap memory classification

Both `alice`, `bob`, `alice->name`, and `bob->name` are heap allocations.
None of them are tied to the lifetime of `main()`'s stack frame; they
persist until explicitly freed, and the only reference to each
`name` block is the single pointer stored inside its parent `Person`
struct.

### 4.4 Cleanup path comparison

**`bob` — freed correctly:**
```c
free(bob->name);   /* frees the string block first */
free(bob);          /* then frees the struct block */
```
Both allocations belonging to `bob` are released. No leak.

**`alice` — freed incorrectly:**
```c
static void person_free_partial(Person *p)
{
    if (!p)
        return;
    free(p);          /* only frees the struct, NOT p->name */
}
...
person_free_partial(alice);
```
`person_free_partial()` only calls `free(p)`. The pointer `p->name`
(address `0x5710eebf76d0`) is never freed. Once `free(p)` executes, the
**only pointer that referenced that string block is destroyed along
with the struct that held it** — there is no other variable anywhere in
the program holding `0x5710eebf76d0`.

### 4.5 Why this is a memory leak, precisely

This block is not "invalid" or "corrupted" — it is a fully valid,
still-allocated heap block. The problem is **ownership loss**: after
`free(alice)`, no reachable pointer in the program refers to
`alice->name`'s block anymore. The block cannot be freed later because
nothing points to it, and it cannot be reused by the allocator until the
process exits. This is the exact definition of a memory leak, and it is
exactly what Valgrind's `LEAK SUMMARY` (`definitely lost` category)
reports — the leaked block is traced back to the `malloc()` call in
`person_new()` at the line allocating `p->name`.

### 4.6 Ownership lesson

The root cause is **inconsistent ownership handling**: `bob`'s cleanup
path knows the struct owns two allocations and frees both; `alice`'s
cleanup path (`person_free_partial`) assumes a struct is a single
allocation and frees only one. The bug is not in `free()` itself — it is
in the calling code failing to track that `Person` "owns" a nested heap
allocation that must be released as part of destroying the struct.

---

## 5. Documented AI Errors

The following errors were produced by an AI assistant when asked to
explain the memory behavior of `aliasing_example.c`, and were corrected
using the real runtime evidence collected above.

### 5.1 Error — Claimed `b = a;` copies the array contents

**AI explanation (incorrect):** The AI stated that `b = a;` copies the
contents of the array into a new memory location, producing two
independent copies of the data.

**Why it was wrong:** The real addresses show `a=0x62798e7426b0` and
`b=0x62798e7426b0` — identical. A content copy would have produced a
distinct address for `b`. Instead, only the pointer value (the address)
was copied.

**Correction:** `b = a;` is a pointer assignment, not a deep copy. `a`
and `b` become aliases of the same single heap block, which is why
freeing through `a` also invalidates `b`.

### 5.2 Error — Claimed `free()` zeroes memory immediately

**AI explanation (incorrect):** The AI stated that `free()` immediately
zeroes the released memory, so a subsequent read through a dangling
pointer would return `0`.

**Why it was wrong:** The actual output was `reading b[2]=-646236049` —
neither `0` nor the original value `22`.

**Correction:** `free()` only updates the allocator's internal
bookkeeping to mark the block reusable; it does not overwrite the
block's contents. Any read afterward is undefined behavior and returns
indeterminate data.

### 5.3 Error — Claimed the use-after-free write causes an immediate crash

**AI explanation (incorrect):** The AI stated that `b[3] = 1234;` would
cause the program to crash immediately, since it writes to memory that
"no longer exists."

**Why it was wrong:** The program ran to completion and printed
`wrote b[3]=1234` with exit code 0 — no crash occurred.

**Correction:** Freed memory remains mapped and accessible in the
process's address space; only its ownership status changes. Writing to
it is undefined behavior that may silently succeed, silently corrupt
unrelated data, or crash — the outcome is not guaranteed, which is why
this class of bug requires a tool like Valgrind to detect reliably.

---

## 6. Summary

| Program | Memory type | Key finding |
|---|---|---|
| `stack_example.c` | Stack | Frames are reused per-call; addresses decrease with recursion depth; each call's locals have independent, function-scoped lifetimes. |
| `aliasing_example.c` | Heap | `b = a;` creates a true alias (same address); `free(a)` invalidates `b` without warning; use-after-free does not reliably crash or zero data. |
| `heap_example.c` | Heap | Each `Person` owns two separate allocations (struct + name string); `person_free_partial()` frees only the struct, leaking `alice->name` because its only owning pointer is destroyed along with it. |

AI-generated explanations were useful as a starting point but contained
three factual errors (Section 5), all disproven using real addresses and
output values captured by compiling and running the programs directly.
This confirms the project's core lesson: AI explanations must be treated
as hypotheses to verify against actual program behavior, not as ground
truth.
