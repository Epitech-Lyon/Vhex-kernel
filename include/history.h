#ifndef __HISTORY_H__
# define __HISTORY_H__

/* Define type. */
#include <types.h>

/* node - history node data part.*/
struct node
{
	const char data[32];
	struct node *next;
};

/* history_s - history data part. */
struct history_s {
	struct node *list;
	off_t offset;
	size_t deep;
};

/* history_update() - create new node into the history list. */
int history_update(struct node **list, const char *data);

/* history_get() - get a node data. */
const void *history_get(struct node *list, off_t offset);

/* history_quit() - free all allocated memory. */
void history_quit(struct node **list);

#endif /*__HISTORY_H__*/
