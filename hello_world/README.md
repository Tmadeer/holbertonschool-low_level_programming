# hello_world

This project is an introduction to the C programming language, the
compilation process, and basic shell scripting. It covers what happens
when C source code is compiled with gcc, how to print text to the
screen, and how to use sizeof to inspect data types.

## Learning Objectives

At the end of this project, you should be able to explain:

- Why C programming is awesome
- Who invented C, and who Dennis Ritchie, Brian Kernighan, and Linus
  Torvalds are
- What happens when you type gcc main.c
- What an entry point is, and what main is
- How to print text using printf, puts, and putchar
- How to get the size of a type using the sizeof operator
- How to compile a C program using gcc
- What the default program name is when compiling with gcc
- What the Betty coding style is, and how to check code with betty-style
- How to find the right header file for a given standard library function
- How the return value of main affects the exit status of a program

## Requirements

- Editors allowed: vi, vim, emacs
- All files compiled on Ubuntu 20.04 LTS with:
  gcc -Wall -Werror -Wextra -pedantic -std=gnu89
- No errors or warnings during compilation
- All files end with a new line
- system is not allowed
- Code follows the Betty style, checked with betty-style.pl and
  betty-doc.pl
- Shell scripts are exactly two lines long and start with #!/bin/bash

## Tasks

| File | Description |
| --- | --- |
| 0-preprocessor | Runs a C file through the preprocessor and saves the result to a file named c |
