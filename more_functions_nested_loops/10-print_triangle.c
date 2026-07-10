#include "main.h"

/**
 * print_triangle - prints a triangle, followed by a new line
 * @size: the size of the triangle
 *
 * Return: void (nothing)
 */
void print_triangle(int size)
{
	int row;
	int spaces;
	int hashes;

	row = 0;
	while (row < size)
	{
		spaces = 0;
		while (spaces < size - 1 - row)
		{
			_putchar(' ');
			spaces++;
		}
		hashes = 0;
		while (hashes <= row)
		{
			_putchar('#');
			hashes++;
		}
		_putchar('\n');
		row++;
	}
	if (size <= 0)
		_putchar('\n');
}
