#include "main.h"

/**
 * sqrt_helper - searches for the natural square root of n, recursively
 * @n: the original number (stays constant)
 * @i: the current candidate being tested
 *
 * Return: i if i * i == n, -1 if no natural square root exists
 */
int sqrt_helper(int n, int i)
{
	if (i * i == n)
	{
		return (i);
	}
	if (i * i > n)
	{
		return (-1);
	}
	return (sqrt_helper(n, i + 1));
}

/**
 * _sqrt_recursion - returns the natural square root of n
 * @n: the number to find the square root of
 *
 * Return: the natural square root of n, or -1 if it doesn't have one
 */
int _sqrt_recursion(int n)
{
	if (n < 0)
	{
		return (-1);
	}
	return (sqrt_helper(n, 1));
}

