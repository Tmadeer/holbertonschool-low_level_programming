#include <stdlib.h>
#include "lists.h"

/**
 * insert_dnodeint_at_index - inserts a new node at a given position
 * @h: pointer to a pointer to the head of the list
 * @idx: index where the new node should be added, starting at 0
 * @n: value to store in the new node
 *
 * Return: the address of the new node, or NULL if it failed
 */
dlistint_t *insert_dnodeint_at_index(dlistint_t **h, unsigned int idx, int n)
{
	dlistint_t *new;
	dlistint_t *prev_node;
	unsigned int len, i;

	if (h == NULL)
		return (NULL);

	len = dlistint_len(*h);

	if (idx > len)
		return (NULL);
	if (idx == 0)
		return (add_dnodeint(h, n));
	if (idx == len)
		return (add_dnodeint_end(h, n));

	prev_node = *h;
	for (i = 0; i < idx - 1; i++)
		prev_node = prev_node->next;

	new = malloc(sizeof(dlistint_t));
	if (new == NULL)
		return (NULL);

	new->n = n;
	new->next = prev_node->next;
	new->prev = prev_node;
	prev_node->next->prev = new;
	prev_node->next = new;

	return (new);
}

