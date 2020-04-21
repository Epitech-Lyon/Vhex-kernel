#include <unistd.h>
#include <stdio.h>
#include <sys/wait.h>
#include "builtin.h"
#include "util.h"

int builtin_proc(int argc, char **argv)
{
	pid_t child;

	// Try to create first child
	(void)argc;
	child = fexecve("/mnt/casio/VHEX/test.elf", argv, NULL);
	if (child == -1)
	{
		printf("fexecve fail :(\n");
		return (0);
	}

	// Wait child process
	wait_child(child);
	return (0);
}
