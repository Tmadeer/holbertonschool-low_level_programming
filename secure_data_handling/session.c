#include <stdlib.h>
#include <string.h>
#include "session.h"

/**
 * session_create - Creates a new session, duplicating the id and data
 * @id: The session id (duplicated internally, must not be NULL)
 * @uid: The user id
 * @data: Optional data buffer to copy (may be NULL if data_len is 0)
 * @data_len: Length of the data buffer
 *
 * Return: A pointer to the new session, or NULL on failure
 */
session_t *session_create(const char *id, unsigned int uid,
			   const unsigned char *data, size_t data_len)
{
	session_t *s;

	if (!id)
		return (NULL);

	s = (session_t *)malloc(sizeof(*s));
	if (!s)
		return (NULL);

	s->id = (char *)malloc(strlen(id) + 1);
	if (!s->id)
	{
		free(s);
		return (NULL);
	}
	strcpy(s->id, id);
	s->uid = uid;

	if (data_len > 0 && data != NULL)
	{
		s->data = (unsigned char *)malloc(data_len);
		if (!s->data)
		{
			free(s->id);
			free(s);
			return (NULL);
		}
		memcpy(s->data, data, data_len);
		s->data_len = data_len;
	}
	else
	{
		s->data = NULL;
		s->data_len = 0;
	}

	return (s);
}

/**
 * session_set_data - Replaces a session's data buffer
 * @s: The session to update
 * @data: The new data (may be NULL if data_len is 0)
 * @data_len: The length of the new data
 *
 * Return: 1 on success, 0 on failure (session left unchanged on failure)
 */
int session_set_data(session_t *s, const unsigned char *data, size_t data_len)
{
	unsigned char *tmp;

	if (!s)
		return (0);

	if (data_len == 0)
	{
		free(s->data);
		s->data = NULL;
		s->data_len = 0;
		return (1);
	}

	tmp = (unsigned char *)realloc(s->data, data_len);
	if (!tmp)
		return (0);

	s->data = tmp;
	memcpy(s->data, data, data_len);
	s->data_len = data_len;
	return (1);
}

/**
 * session_destroy - Frees a session and all its owned memory
 * @s: The session to destroy (NULL is a no-op)
 */
void session_destroy(session_t *s)
{
	if (!s)
		return;
	free(s->id);
	free(s->data);
	free(s);
}

