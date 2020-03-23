#include <lib/unistd.h>
#include <lib/display.h>
#include <lib/stdio.h>
#include <builtin.h>

int builtin_proc(void)
{
	pid_t child;
	int wstatus;

	puts("proc test entry :)\n");
	printf("  PPID   PID  PGID\n");
	printf("%-6d%-6d%-6d\n", getppid(), getpid(), getpgid());

	// Try to create first child
	child = fexecve("/mnt/casio/VHEX/test.elf");
	if (child == -1)
	{
		printf("fexecve fail :(\n");
		return (0);
	}

	// Wait child death
	waitpid(-1, &wstatus, 0);
	printf("+++ exited with %d +++\n", WEXITSTATUS(wstatus));
	return (0);
}
