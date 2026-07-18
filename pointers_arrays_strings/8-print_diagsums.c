#include "main.h"
#include <stdio.h>

/**
 * print_diagsums - prints the sum of the two diagonals of a square matrix
 * @a: pointer to the flat-mapped integer matrix
 * @size: the size of the square matrix (width/height)
 *
 * Return: void
 */
void print_diagsums(int *a, int size)
{
	int i;
	int sum1;
	int sum2;

	sum1 = 0;
	sum2 = 0;
	i = 0;
	while (i < size)
	{
		/* Element on the primary diagonal: row i, column i */
		sum1 = sum1 + a[i * size + i];

		/* Element on the secondary diagonal: row i, column (size - 1 - i) */
		sum2 = sum2 + a[i * size + (size - 1 - i)];

		i++;
	}

	printf("%d, %d\n", sum1, sum2);
}
