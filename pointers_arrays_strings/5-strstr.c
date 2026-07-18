#include "main.h"
#include <stddef.h>

/**
 * _strstr - locates a substring
 * @haystack: the main string to search within
 * @needle: the substring to locate
 *
 * Return: a pointer to the beginning of the located substring,
 *         or NULL if the substring is not found
 */
char *_strstr(char *haystack, char *needle)
{
	int i;
	int j;

	if (needle[0] == '\0')
	{
		return (haystack);
	}

	i = 0;
	while (haystack[i] != '\0')
	{
		j = 0;
		while (needle[j] != '\0' && haystack[i + j] == needle[j])
		{
			j++;
		}

		if (needle[j] == '\0')
		{
			return (haystack + i);
		}

		i++;
	}

	return (NULL);
}
