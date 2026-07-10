#include "main.h"

/**
 * print_diagonal - draws a diagonal line on the terminal
 * @n: the number of times to print the character '\'
 *
 * Return: void (nothing)
 */
void print_diagonal(int n)
{
	int row;
	int spaces;

	row = 0;
	while (row < n)
	{
		spaces = 0;
		while (spaces < row)
		{
			_putchar(' ');
			spaces++;
		}
		_putchar('\\');
		_putchar('\n');
		row++;
	}
	if (n <= 0)
		_putchar('\n');
}
