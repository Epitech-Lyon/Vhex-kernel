#include <criterion/criterion.h>
#include "tests/internal.h"
#include <errno.h>

// Internal prototypes.
extern int strtotab(int *argc, char ***argv, char const *str);
extern void strtotab_quit(int *argc, char ***argv);

Test(strtotab, quote)
{
	char **argv;
	int argc;
	int ret;

	ret = strtotab(&argc, &argv, "hey? \"What's \nup?\" yo!\"\n");
	cr_expect_eq(ret, 0);
	cr_expect_eq(argc, 3);
	cr_assert_str_eq(argv[0], "hey?");
	cr_assert_str_eq(argv[1], "What's \nup?");
	cr_assert_str_eq(argv[2], "yo!\"");
}

Test(strtotab, noword)
{
	char **argv;
	int argc;
	int ret;

	ret = strtotab(&argc, &argv, "");
	cr_expect_eq(ret, EINVAL);
	cr_expect_eq(argc, 0);
	cr_assert_null(argv);
}

Test(strtotab, entry_error)
{
	char **argv;
	int argc;
	int ret;

	ret = strtotab(NULL, &argv, "aaaaaaaaa aa");
	cr_expect_eq(ret, EFAULT);
	ret = strtotab(&argc, NULL, "aaaaaaaaa aa");
	cr_expect_eq(ret, EFAULT);
	ret = strtotab(&argc, &argv, NULL);
	cr_expect_eq(ret, EFAULT);
}

Test(strtotab, spaces_tabs)
{
	char **argv;
	int argc;
	int ret;

	ret = strtotab(&argc, &argv, "hey?            What's\t\t \t  up?");
	cr_expect_eq(ret, 0);
	cr_expect_eq(argc, 3);
	cr_assert_str_eq(argv[0], "hey?");
	cr_assert_str_eq(argv[1], "What's");
	cr_assert_str_eq(argv[2], "up?");
}

Test(strtotab, normal)
{
	char **argv;
	int argc;
	int ret;

	ret = strtotab(&argc, &argv, "hey? What's up?\n");
	cr_expect_eq(ret, 0);
	cr_assert_eq(argc, 3);
	cr_assert_str_eq(argv[0], "hey?");
	cr_assert_str_eq(argv[1], "What's");
	cr_assert_str_eq(argv[2], "up?");
}

Test(strtotab, alloc_fails)
{
	char **argv;
	int argc;
	int ret;

	malloc_hook_update(FAIL_NEXT);
	ret = strtotab(&argc, &argv, "hey? \"What's up?");
	cr_assert_eq(ret, ENOMEM);
	malloc_hook_update(FAIL_NEXT_DYN, 2);
	ret = strtotab(&argc, &argv, "hey? \"What's up?");
	cr_assert_eq(ret, ENOMEM);
	malloc_hook_update(FAIL_NEXT_DYN, 2);
	ret = strtotab(&argc, &argv, "\"What's\" up?");
	cr_assert_eq(ret, ENOMEM);

}

Test(strtotab, quit)
{
	char **argv;
	int argc;

	strtotab(&argc, &argv, "hey? What's up?");
	strtotab_quit(&argc, &argv);
	cr_assert_eq(argc, 0);
	cr_assert_null(argv);
	strtotab_quit(NULL, &argv);
	strtotab_quit(&argc, NULL);
	argv = NULL;
	strtotab_quit(&argc, &argv);
}
