#include <lib/unistd.h>
#include <lib/display.h>
#include <lib/stdio.h>
#include <builtin.h>

int builtin_proc(void)
{
	pid_t child;
	int wstatus;

	puts("proc test entry :)\n");
	printf("printf test:\n");
	printf("0#10x: %0#10x\n", 0xabcdef);
	printf("str: %s\n", "Oui le test oui ! :D");
	printf("interger: %d\n", 12345);
	printf("interger: %lld\n", 999999999999999999);

	// Try to create first child
	/*child = fexecve("/mnt/casio/VHEX/test.elf");
	if (child == -1)
	{
		puts("fexecve fail :(\n");
		return (0);
	}

	// Wait child death
	waitpid(child, &wstatus, WUNTRACED);
*/	return (0);
}
