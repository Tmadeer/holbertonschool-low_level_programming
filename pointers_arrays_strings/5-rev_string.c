#include "main.h"

/**
 * rev_string - reverses a string
 * @s: the string to reverse
 *
 * Return: void (nothing)
 */
void rev_string(char *s)
{
	int len;
	int start;
	int end;
	char temp;

	len = 0;
	while (s[len] != '\0')
		len++;

	start = 0;
	end = len - 1;
	while (start < end)
	{
		temp = s[start];
		s[start] = s[end];
		s[end] = temp;
		start++;
		end--;
	}
}
