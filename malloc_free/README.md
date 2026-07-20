# 0x0A. C - malloc, free

## Description
This project covers dynamic memory allocation in C using `malloc` and `free`. It explores the differences between static/automatic memory allocation and dynamic memory allocation on the Heap, along with handling memory leaks using `Valgrind`.

## Requirements
* Allowed editors: `vi`, `vim`, `emacs`
* All files will be compiled on Ubuntu 20.04 LTS using `gcc` with flags: `-Wall -Werror -Wextra -pedantic -std=gnu89`
* Code should follow the **Betty** coding style.
* No global variables allowed.
* Maximum 5 functions per file.
* The only standard library functions allowed are `malloc` and `free`.

## File Descriptions

| File | Description |
| --- | --- |
| `main.h` | Header file containing prototypes for all functions and `_putchar`. |
| `0-create_array.c` | Function that creates an array of chars and initializes it with a specific char. |

## Compilation & Execution
To compile the files, use the following command:

```bash
gcc -Wall -Werror -Wextra -pedantic -std=gnu89 0-main.c 0-create_array.c -o create_array
