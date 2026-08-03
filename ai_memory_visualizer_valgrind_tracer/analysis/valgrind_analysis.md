# Valgrind Analysis — AI Memory Tracer

## 1. Methodology

All three programs were compiled using the provided Makefile
(`-Wall -Wextra -Werror -pedantic -std=gnu89 -g`) and analyzed with:

```bash
valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes ./<binary>
```

- `--leak-check=full`: reports full details for every leaked block,
  including the allocation call stack.
- `--show-leak-kinds=all`: reports every leak category (definitely lost,
  indirectly lost, possibly lost, still reachable), not just a summary
  count.
- `--track-origins=yes`: traces the origin of uninitialized values when
  applicable.

Programs analyzed: `aliasing_example.c`, `heap_example.c`,
`crash_example.c`. Raw output for each run is saved under
`analysis/valgrind_raw_*.txt`.

Environment: WSL (Ubuntu), Valgrind 3.18.1.

---

## 2. `aliasing_example.c` — Findings

### 2.1 Program output during the run
```
a=0x4a8d480 b=0x4a8d480 a[2]=22 b[2]=22
after free(a): b=0x4a8d480 (dangling)
reading b[2]=22
wrote b[3]=1234
```

### 2.2 Valgrind reported 3 errors, all use-after-free

**Error 1 — Invalid read (line 42)**
```
Invalid read of size 4
   at 0x1092F5: main (aliasing_example.c:42)
Address 0x4a8d488 is 8 bytes inside a block of size 20 free'd
   at free (aliasing_example.c:38)
Block was alloc'd at malloc (aliasing_example.c:12), called from main (aliasing_example.c:30)
```
| Field | Value |
|---|---|
| Type | Invalid read — use-after-free |
| Object | `b[2]`, i.e. offset 8 bytes into the 20-byte block originally allocated for `a` |
| Root cause | The block (5 `int`s = 20 bytes) was freed at line 38 (`free(a)`). Line 42 reads through `b`, which still holds the same address as `a`. Reading through it after the free is a use-after-free read: the block is no longer owned by the program, even though `b`'s value is unchanged. |

**Error 2 — Invalid write (line 44)**
```
Invalid write of size 4
   at main (aliasing_example.c:44)
Address 0x4a8d48c is 12 bytes inside a block of size 20 free'd
```
| Field | Value |
|---|---|
| Type | Invalid write — use-after-free |
| Object | `b[3]`, offset 12 bytes into the same freed block |
| Root cause | `b[3] = 1234;` writes to memory no longer owned by the program. This is more severe than a read: if the allocator had reused this block for a new allocation, this write would silently corrupt unrelated data. |

**Error 3 — Invalid read (line 45)**
```
Invalid read of size 4
   at main (aliasing_example.c:45)
```
| Field | Value |
|---|---|
| Type | Invalid read — use-after-free |
| Object | `b[3]`, reading back the value just written, to print it |
| Root cause | Same freed block as Error 2; reading it to confirm the write is still an invalid access regardless of whether the write "appeared" to succeed. |

### 2.3 Heap summary
```
in use at exit: 0 bytes in 0 blocks
All heap blocks were freed -- no leaks are possible
```

This is an important distinction to record explicitly: `aliasing_example.c`
has **zero leaks**. Every allocated block was eventually freed exactly
once (`free(a)`). The entire problem here is invalid *access* to memory
after its lifetime ended — not lost ownership. This separates it clearly
from the leak category found in `heap_example.c` below.

### 2.4 Connection to Task 1 memory map

In the manual analysis (Task 1), reading `b[2]` after `free(a)` returned
different garbage values across separate runs (`-646236049` in one run,
`22` in this run). Valgrind confirms formally why: the read is flagged
as "invalid" precisely because the memory's content is undefined once
freed — the value observed is not meaningful and must not be relied
upon. Valgrind does not just detect *that* something is wrong; it proves
*why*, by tracing the exact `malloc`/`free`/access call chain.

---

## 3. `heap_example.c` — Findings

### 3.1 Program output during the run
```
alice=0x4a8d480 name=0x4a8d4d0 age=30
bob=0x4a8d520 name=0x4a8d570 age=41
```

### 3.2 Valgrind report
```
6 bytes in 1 blocks are definitely lost in loss record 1 of 1
   at malloc (in vgpreload_memcheck-amd64-linux.so)
   by 0x109211: person_new (heap_example.c:21)
   by 0x1092FA: main (heap_example.c:51)

LEAK SUMMARY:
   definitely lost: 6 bytes in 1 blocks
   indirectly lost: 0 bytes in 0 blocks
     possibly lost: 0 bytes in 0 blocks
   still reachable: 0 bytes in 0 blocks
```

| Field | Value |
|---|---|
| Type | Memory leak — **definitely lost** (the strictest category: no pointer anywhere in the program, direct or indirect, still references this block) |
| Object | `alice->name` — confirmed by the exact size: `"Alice"` is 5 characters + 1 null terminator = **6 bytes**, matching the report precisely |
| Allocation site | `heap_example.c:21` — `p->name = malloc(len + 1);` inside `person_new()` |
| Called from | `heap_example.c:51` — `alice = person_new("Alice", 30);` |
| Root cause | `person_free_partial(alice)` only executes `free(p)`, releasing the `Person` struct but never `free(p->name)`. The moment `free(p)` runs, the struct holding the only pointer to that 6-byte block is destroyed. No other variable in the program ever held that address. The block becomes permanently unreachable — this is a leak due to **lost ownership**, not corrupted memory. |

### 3.3 Why "definitely" and not "possibly" lost

Valgrind distinguishes leak severity based on reachability analysis.
"Definitely lost" means Valgrind found **no pointer chain whatsoever**
— direct or indirect — leading to the block at program exit. This
applies here because `alice->name` had exactly one owning pointer
(`alice->name` itself), and that pointer was destroyed along with its
container without ever being read or copied elsewhere.

### 3.4 Comparison confirming the root cause is in the cleanup code

Only **one block** is reported as leaked, not two. This confirms `bob`
was released correctly:
```c
free(bob->name);   /* releases the 6-byte name block for "Bob" */
free(bob);          /* releases the struct */
```
Both paths call `malloc()` identically inside `person_new()`; the
divergence is entirely in the cleanup logic (`person_free_partial()`
vs. the explicit two-step free for `bob`). This confirms the leak is
caused by inconsistent ownership handling in the calling code, not by
`malloc`/`free` themselves.

---

## 4. `crash_example.c` — Findings

### 4.1 Program output during the run
```
crash_example: deterministic NULL dereference (segmentation fault)
  requesting n=0
Segmentation fault (core dumped)
```

### 4.2 Valgrind report — primary error
```
Invalid write of size 4
   at 0x10928A: main (crash_example.c:32)
Address 0x0 is not stack'd, malloc'd or (recently) free'd

Process terminating with default action of signal 11 (SIGSEGV)
Access not within mapped region at address 0x0
```

| Field | Value |
|---|---|
| Type | Invalid write — **NULL pointer dereference**, not use-after-free |
| Key distinction | Valgrind explicitly states the address `0x0` "is not stack'd, malloc'd or (recently) free'd" — meaning this address never had a valid memory region backing it at all. This differs fundamentally from `aliasing_example`'s errors, where the address *was* a real, previously-valid heap address that had simply been freed. |
| Object | `nums`, at `nums[0] = 42;` (line 32) |
| Root cause | `allocate_numbers(0)` is called with `n=0`. Inside it, `if (n <= 0) return NULL;` correctly returns `NULL`. `main()` never checks this return value before dereferencing it: `nums[0] = 42;` is equivalent to `*(nums + 0) = 42`, i.e. a direct write to address `0x0`. This is not a lifetime violation — it is a **missing NULL-check** before pointer use. |
| System behavior | The OS delivers `SIGSEGV` (signal 11) and terminates the process immediately, because address `0x0` falls entirely outside any memory region mapped to the process. This is why this bug crashes deterministically and immediately — unlike the `aliasing_example` use-after-free errors, which did not crash the program at all. |

### 4.3 Secondary report — "still reachable" (not a bug in this code)
```
1,024 bytes in 1 blocks are still reachable in loss record 1 of 1
   at malloc ...
   by _IO_file_doallocate ...
   by puts (crash_example.c:27)
```

| Field | Value |
|---|---|
| Type | "still reachable" — the weakest leak category |
| Root cause | This is an internal buffer allocated by the C standard library (`stdio`) the first time `puts()`/`printf()` is used, to buffer output. It is not allocated by application code and is not evidence of a bug in `crash_example.c`. It is included here only to document that not every entry in a Valgrind report is a programmer error — "still reachable" blocks are typically expected library behavior and must be classified separately from "definitely lost" leaks. |

---

## 5. Documented AI Error

**AI explanation (incorrect):** When asked to interpret the
`aliasing_example.c` Valgrind report, the AI stated that all three
reported errors (lines 42, 44, 45) were "leaks," and recommended adding
more `free()` calls to fix them.

**Why it was wrong:** The Valgrind `HEAP SUMMARY` for this program
explicitly states:
```
in use at exit: 0 bytes in 0 blocks
All heap blocks were freed -- no leaks are possible
```
There is no leak in this program at all — every block allocated was
freed exactly once. The three reported errors are all classified by
Valgrind as **"Invalid read"/"Invalid write"**, which is a fundamentally
different category from a leak: a leak means memory was *never freed*;
these errors mean memory *was* freed and then accessed anyway
(use-after-free). Adding another `free()` call, as the AI suggested,
would not fix anything — it would in fact cause a **double-free**, a
separate and equally serious bug.

**Correction:** The three errors are use-after-free reads/writes on the
same already-freed 20-byte block (freed once, correctly, at line 38).
The actual fix (not required by this task, but worth noting for
correctness) would be to stop accessing `b` after `free(a)`, e.g. by
setting `b = NULL;` immediately after the free and never dereferencing
it afterward — not by adding a second `free()`.

---

## 6. Summary Table

| Program | Error type | Valgrind category | Line(s) | Root cause |
|---|---|---|---|---|
| `aliasing_example.c` | Invalid read/write ×3 | Use-after-free | 42, 44, 45 | `b` retains the address of a block freed via alias `a` at line 38; no lifetime tracking prevents further access through `b`. |
| `heap_example.c` | Memory leak | Definitely lost (6 bytes) | alloc: 21, leaked via: 51 | `person_free_partial()` frees only the `Person` struct, destroying the only pointer to `p->name` without freeing it first. |
| `crash_example.c` | Invalid write | NULL pointer dereference (SIGSEGV) | 32 | `allocate_numbers(0)` returns `NULL`; the return value is dereferenced without a NULL-check. |
| `crash_example.c` (secondary) | "Still reachable" | Not a bug | n/a | Internal `stdio` buffer allocated by `puts()`; expected library behavior, not application-level leak. |

All three programs' Valgrind reports independently confirm the manual
memory-lifetime analysis performed in Task 1 (`memory_maps.md`),
demonstrating that the AI-assisted mental model built there matches
real runtime tool output once cross-checked and corrected.
