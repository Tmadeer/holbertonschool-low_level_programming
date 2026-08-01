#include <stdio.h>
#include "main.h"

/**
 * main - Prints the program's name, followed by a new line.
 * @argc: The number of command line arguments.
 * @argv: An array containing the program command line arguments.
 *
 * Return: Always 0.
 */
int main(int argc, char *argv[])
{
	(void)argc;

	printf("%s\n", argv[0]);

	return (0);
}

