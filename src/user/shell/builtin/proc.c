#include <lib/unistd.h>
#include <lib/display.h>
#include <lib/stdio.h>
#include <builtin.h>

int builtin_proc(void)
{
	pid_t child;
	int wstatus;

	puts("proc test entry :)\n");

	// Try to create first child
	child = fexecve("/mnt/casio/VHEX/test.elf");
	if (child == -1)
	{
		puts("fexecve fail :(\n");
		return (0);
	}

	// Wait child death
	waitpid(child, &wstatus, WUNTRACED);
	return (0);
}
