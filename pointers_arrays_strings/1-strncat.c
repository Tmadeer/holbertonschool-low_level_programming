#include "main.h"

/**
 * _strncat - concatenates two strings using at most n bytes from src
 * @dest: the destination string
 * @src: the source string to append
 * @n: the maximum number of bytes to use from src
 *
 * Return: a pointer to dest
 */
char *_strncat(char *dest, char *src, int n)
{
	int dest_len;
	int i;

	dest_len = 0;
	while (dest[dest_len] != '\0')
		dest_len++;

	i = 0;
	while (i < n && src[i] != '\0')
	{
		dest[dest_len + i] = src[i];
		i++;
	}
	dest[dest_len + i] = '\0';

	return (dest);
}
