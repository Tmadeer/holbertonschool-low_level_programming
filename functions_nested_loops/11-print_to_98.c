#include "main.h"
#include <stdio.h>

/**
 * print_to_98 - prints all integers from n to 98, followed by a new line
 * @n: the starting number
 *
 * Return: void (nothing)
 */
void print_to_98(int n)
{
	if (n <= 98)
	{
		while (n <= 98)
		{
			if (n != 98)
				printf("%d, ", n);
			else
				printf("%d", n);
			n++;
		}
	}
	else
	{
		while (n >= 98)
		{
			if (n != 98)
				printf("%d, ", n);
			else
				printf("%d", n);
			n--;
		}
	}
	printf("\n");
}
