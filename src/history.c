#ifndef DEBUG
# include <history.h>
# include <syscalls.h>
#else
# include "history.h"
# include <stdlib.h>
#endif
#include <string.h>
#include <errno.h>

// TODO:write docs !
// *-*
// |A| --> 
// *-*
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

const void *history_get(struct node *list, off_t offset)
{
	if (list == NULL)
		return (NULL);
	if (offset == 0)
		return (list->data);
	return (history_get(list->next, offset - 1));
}

void history_quit(struct node **list)
{
	if (list == NULL || *list == NULL)
		return;
	history_quit(&(*list)->next);
	free(*list);
	*list = NULL;
}
