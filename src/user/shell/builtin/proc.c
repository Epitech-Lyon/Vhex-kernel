#include <unistd.h>
#include <stdio.h>
#include <sys/wait.h>
#include "builtin.h"

int builtin_proc(void)
{
	char *argv[2] = {"test", NULL};
	pid_t child;
	int wstatus;

	//puts("proc test entry :)\n");
	//printf("  PPID   PID  PGID\n");
	//printf("%-6d%-6d%-6d\n", getppid(), getpid(), getpgid());

	// Try to create first child
	child = fexecve("/mnt/casio/VHEX/test.elf", argv, NULL);
	if (child == -1)
	{
		printf("fexecve fail :(\n");
		return (0);
	}

	// Wait child death
	waitpid(child, &wstatus, 0);
	printf("+++ exited with %d +++\n", WEXITSTATUS(wstatus));
	return (0);
}
