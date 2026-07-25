#include "main.h"
#include <stdlib.h>

/**
 * _realloc - reallocates a memory block using malloc and free
 * @ptr: pointer to the memory previously allocated
 * @old_size: size, in bytes, of the allocated space for ptr
 * @new_size: new size, in bytes of the new memory block
 *
 * Return: pointer to newly allocated memory, or NULL if it fails
 */
void *_realloc(void *ptr, unsigned int old_size, unsigned int new_size)
{
	char *new_ptr;
	char *old_ptr;
	unsigned int i, limit;

	if (new_size == old_size)
		return (ptr);

	if (ptr == NULL)
		return (malloc(new_size));

	if (new_size == 0 && ptr != NULL)
	{
		free(ptr);
		return (NULL);
	}

	new_ptr = malloc(new_size);
	if (new_ptr == NULL)
		return (NULL);

	old_ptr = ptr;

	if (new_size < old_size)
		limit = new_size;
	else
		limit = old_size;

	for (i = 0; i < limit; i++)
		new_ptr[i] = old_ptr[i];

	free(ptr);

	return (new_ptr);
}

