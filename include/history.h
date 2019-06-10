#ifndef __HISTORY_H__
# define __HISTORY_H__

#include <types.h>

struct node
{
	const char data[32];
	struct node *next;
};

struct history_s {
	struct node *list;
	off_t offset;
	size_t deep;
};

int history_update(struct node **list, const char *data);
const void *history_get(struct node *list, off_t offset);
void history_quit(struct node **list);
#endif /*__HISTORY_H__*/
