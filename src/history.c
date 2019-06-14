#ifndef DEBUG
# include <history.h>
# include <syscalls.h>
#else
# include "history.h"
# include <stdlib.h>
#endif
#include <string.h>
#include <errno.h>

//---
// history_update()
// Add new history node to the chained list. This is a singly
// linked list, but the node insertion from the beginning.
//
// Exemple: add node A, B, C and D
// 1. A -> NULL
// 2. B -> A -> NULL
// 3. C -> B -> A -> NULL
// 4. D -> C -> B -> A -> NULL
//---
int history_update(struct node **list, const char *data)
{
	void *swap;

	if (list == NULL)
		return (EFAULT);
	swap = *list;
	*list = (struct node *)malloc(sizeof(struct node));
	if (*list == NULL){
		*list = swap;
		return (ENOMEM);
	}
	strcpy((void*)(*list)->data, data);
	(*list)->next = swap;
	return (0);
}

//
// history_get()
// primitive function which will return the data stored in
// the "offset" position in the list.
//
const void *history_get(struct node *list, off_t offset)
{
	if (list == NULL)
		return (NULL);
	if (offset == 0)
		return (list->data);
	return (history_get(list->next, offset - 1));
}

//
// history_quit()
// Free all allocated memory.
//
void history_quit(struct node **list)
{
	if (list == NULL || *list == NULL)
		return;
	history_quit(&(*list)->next);
	free(*list);
	*list = NULL;
}
