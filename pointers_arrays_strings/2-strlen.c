#include "main.h"

/**
 * _strlen - returns the length of a string
 * @s: the string to measure
 *
 * Return: the number of characters in s, not counting the null byte
 */
int _strlen(char *s)
{
	int length;

	length = 0;
	while (s[length] != '\0')
		length++;
	return (length);
}
