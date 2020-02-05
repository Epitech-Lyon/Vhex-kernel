#include <lib/unistd.h>
#include <lib/display.h>
#include <builtin.h>

int builtin_proc(void)
{
	pid_t child;

	// Debug
	dclear();
	dprint(0, 0, "Proc builtin !");
	dupdate();
	for (int i = 0 ; i < 3000000 ; i++);

	// Try to create first child
	/*child = fork();
	if (child == -1)
	{
		dprint(0, 1, "fork() fail !");
		dupdate();
		for (int i = 0 ; i < 3000000 ; i++);
		return (0);
	}

	// Check parent
	if (child_pid != 0)
	{
		int counter = 0;
		while (1)
		{
			dprint(0, 1, "parent: %d", counter++);
			for (int i = 0 ; i < 3000000 ; i++);
		}
	} else {
		int counter = 0;
		while (1)
		{
			dprint(0, 2, "child: %d", counter++);
			for (int i = 0 ; i < 9000000 ; i++);
		}
	}*/
	return (0);
}
