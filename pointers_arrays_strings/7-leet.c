#include "main.h"

/**
 * leet - encodes a string into 1337
 * @str: the string to encode
 *
 * Return: a pointer to the encoded string
 */
char *leet(char *str)
{
	int i;
	int j;
	char letters[] = "aAeEoOtTlL";
	char leet_chars[] = "4433007711";

	i = 0;
	while (str[i] != '\0')
	{
		j = 0;
		while (j < 10)
		{
			if (str[i] == letters[j])
			{
				str[i] = leet_chars[j];
			}
			j++;
		}
		i++;
	}

	return (str);
}
