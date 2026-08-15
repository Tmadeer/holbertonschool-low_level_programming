#include <stdlib.h>
#include <string.h>
#include "store.h"

/**
 * store_init - Initializes an empty store
 * @st: The store to initialize
 */
void store_init(store_t *st)
{
	if (st)
		st->head = NULL;
}

/**
 * node_create - Allocates a new list node wrapping a session
 * @s: The session to wrap
 *
 * Return: A pointer to the new node, or NULL on failure
 */
static node_t *node_create(session_t *s)
{
	node_t *n = (node_t *)malloc(sizeof(*n));

	if (!n)
		return (NULL);
	n->sess = s;
	n->next = NULL;
	return (n);
}

/**
 * store_add - Adds a session to the store (takes ownership of s)
 * @st: The store
 * @s: The session to add
 *
 * Return: 1 on success, 0 on failure. On any failure the session
 * is destroyed internally to avoid leaking it.
 */
int store_add(store_t *st, session_t *s)
{
	node_t *n, *cur;

	if (!st || !s || !s->id)
	{
		session_destroy(s);
		return (0);
	}

	cur = st->head;
	while (cur)
	{
		if (cur->sess && cur->sess->id &&
		    strcmp(cur->sess->id, s->id) == 0)
		{
			session_destroy(s);
			return (0);
		}
		cur = cur->next;
	}

	n = node_create(s);
	if (!n)
	{
		session_destroy(s);
		return (0);
	}

	n->next = st->head;
	st->head = n;
	return (1);
}

/**
 * store_get - Looks up a session by id
 * @st: The store
 * @id: The id to search for
 *
 * Return: A pointer to the session, or NULL if not found
 */
session_t *store_get(store_t *st, const char *id)
{
	node_t *cur;

	if (!st || !id)
		return (NULL);

	cur = st->head;
	while (cur)
	{
		if (cur->sess && cur->sess->id && strcmp(cur->sess->id, id) == 0)
			return (cur->sess);
		cur = cur->next;
	}
	return (NULL);
}

/**
 * store_delete - Removes a session by id from the store
 * @st: The store
 * @id: The id to remove
 * @out: If not NULL, receives ownership of the removed session
 *
 * Return: 1 if deleted, 0 if not found
 */
int store_delete(store_t *st, const char *id, session_t **out)
{
	node_t *cur, *prev;

	if (!st || !id)
		return (0);

	prev = NULL;
	cur = st->head;
	while (cur)
	{
		if (cur->sess && cur->sess->id && strcmp(cur->sess->id, id) == 0)
		{
			if (prev)
				prev->next = cur->next;
			else
				st->head = cur->next;

			if (out)
				*out = cur->sess;
			else
				session_destroy(cur->sess);

			free(cur);
			return (1);
		}
		prev = cur;
		cur = cur->next;
	}
	return (0);
}

/**
 * store_destroy - Frees every session and node in the store
 * @st: The store to clear (head is reset to NULL)
 */
void store_destroy(store_t *st)
{
	node_t *cur, *next;

	if (!st)
		return;

	cur = st->head;
	while (cur)
	{
		next = cur->next;
		session_destroy(cur->sess);
		free(cur);
		cur = next;
	}
	st->head = NULL;
}

