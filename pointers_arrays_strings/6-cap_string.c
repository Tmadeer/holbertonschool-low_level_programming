#include "main.h"

/**
 * cap_string - capitalizes all words of a string
 * @str: the string to modify
 *
 * Return: a pointer to the modified string
 */
char *cap_string(char *str)
{
	int i;
	int j;
	char separators[] = " \t\n,;.!?\"(){}";

	i = 0;
	while (str[i] != '\0')
	{
		/* Capitalize the first character of the string if it is lowercase */
		if (i == 0 && str[i] >= 'a' && str[i] <= 'z')
		{
			str[i] = str[i] - 32;
		}

		/* Check if the current character follows a separator */
		j = 0;
		while (separators[j] != '\0')
		{
			if (str[i] == separators[j])
			{
				if (str[i + 1] >= 'a' && str[i + 1] <= 'z')
				{
					str[i + 1] = str[i + 1] - 32;
				}
			}
			j++;
		}
		i++;
	}

	return (str);
}
