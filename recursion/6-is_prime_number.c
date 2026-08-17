#include "main.h"

/**
 * prime_helper - checks if n has a divisor i, recursively
 * @n: the number being tested
 * @i: the current candidate divisor
 *
 * Return: 0 if a divisor is found, 1 if n is prime
 */
int prime_helper(int n, int i)
{
	if (i * i > n)
	{
		return (1);
	}
	if (n % i == 0)
	{
		return (0);
	}
	return (prime_helper(n, i + 1));
}

/**
 * is_prime_number - checks if n is a prime number
 * @n: the number to check
 *
 * Return: 1 if n is prime, 0 otherwise
 */
int is_prime_number(int n)
{
	if (n <= 1)
	{
		return (0);
	}
	return (prime_helper(n, 2));
}

