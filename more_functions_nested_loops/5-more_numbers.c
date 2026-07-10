#include "main.h"

/**
 * more_numbers - prints 0 to 14, ten times, each followed by a new line
 *
 * Return: void (nothing)
 */
void more_numbers(void)
{
	int row;
	int n;

	row = 0;
	while (row < 10)
	{
		n = 0;
		while (n <= 14)
		{
			if (n < 10)
				_putchar(n + '0');
			else
				_putchar(n / 10 + '0');
			if (n >= 10)
				_putchar(n % 10 + '0');
			n++;
		}
		_putchar('\n');
		row++;
	}
}
