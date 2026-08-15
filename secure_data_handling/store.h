#ifndef STORE_H
#define STORE_H
#include "session.h"

typedef struct node_s {
	session_t *sess;
	struct node_s *next;
} node_t;

typedef struct store_s {
	node_t *head;
} store_t;

void store_init(store_t *st);

/*
 * Adds a session to the store. The store takes ownership of s in
 * every case: on success the session is now owned by the store;
 * on failure (duplicate id, allocation failure, or invalid args)
 * the session is destroyed internally so the caller must not use
 * or free it again.
 * Returns 1 if added, 0 otherwise.
 */
int store_add(store_t *st, session_t *s);

session_t *store_get(store_t *st, const char *id);

/*
 * Deletes a session by id.
 * If out is not NULL, ownership of the removed session is
 * transferred to the caller via *out (the caller must eventually
 * call session_destroy on it). If out is NULL, the session is
 * destroyed internally.
 * Returns 1 if deleted, 0 if not found.
 */
int store_delete(store_t *st, const char *id, session_t **out);

void store_destroy(store_t *st);
#endif

