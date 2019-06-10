#include <criterion/criterion.h>
#include "tests/internal.h"
#include "history.h"
#include <errno.h>


//TODO: write doc.

Test(history, add)
{
	struct node *list;

	list = NULL;
	cr_assert_eq(history_update(&list, "ls -l"), 0);
	cr_assert_eq(history_update(&list, "cat -e"), 0);
	cr_assert_eq(history_update(NULL, "cat -e"), EFAULT);
	// malloc fails.
	malloc_hook_update(FAIL_NEXT);
	cr_assert_eq(history_update(&list, "cat -e"), ENOMEM);

	// Check info.
	cr_assert_str_eq(list->data, "cat -e");
	cr_assert_str_eq(list->next->data, "ls -l");
	cr_assert_null(list->next->next);
}

Test(history, get)
{
	struct node *list;

	list = NULL;
	history_update(&list, "ls -l");
	history_update(&list, "cat -e");
	cr_assert_str_eq(history_get(list, 1), "ls -l");
	cr_assert_str_eq(history_get(list, 0), "cat -e");
	cr_assert_null(history_get(list, 4));
}

Test(history, quit)
{
	struct node *list;

	list = NULL;
	history_quit(&list);
	history_update(&list, "ls -l");
	history_update(&list, "cat -e");
	history_update(&list, "test");
	history_update(&list, "yes");
	history_quit(&list);
	history_quit(NULL);
	cr_assert_null(list);
}
