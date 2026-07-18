#include "main.h"

/**
 * _strspn - gets the length of a prefix substring
 * @s: the initial string to evaluate
 * @accept: the string containing list of bytes to match
 *
 * Return: the number of bytes in the initial segment of s matching accept
 */
unsigned int _strspn(char *s, char *accept)
{
	unsigned int count;
	int i;
	int j;
	int match;

	count = 0;
	i = 0;
	while (s[i] != '\0')
	{
		match = 0;
		j = 0;
		while (accept[j] != '\0')
		{
			if (s[i] == accept[j])
			{
				match = 1;
			}
			j++;
		}

		if (match == 0)
		{
			return (count);
		}

		count++;
		i++;
	}

	return (count);
}
