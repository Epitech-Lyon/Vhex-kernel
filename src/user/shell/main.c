#include <unistd.h>
#include <string.h>
#include <stdio.h>
#include "util.h"

static int manual_proc_call(char **argv)
{
	char pathname[32];
	int wstatus;
	pid_t child;

	// Generate pathname
	// TODO: handle PATH
	strcpy(pathname, "/mnt/casio/VHEX/");
	strcat(pathname, argv[0]);

	// Try to call binary from eeprom
	printf("try to call '%s'\n", pathname);
	child = fexecve(pathname, argv, NULL);
	if (child != 0)
		return (-1);

	waitpid(child, &wstatus, 0);
	return (WEXITSTATUS(wstatus));
}

//TODO: documentation.
int main(void)
{
	char input[12];
	int cmd_size;
	char **argv;
	int argc;

	// Shell main loop.
	write(STDOUT_FILENO, "Boot Complete !\n", 16);
	while (1)
	{
		// Get user command.
		write(STDOUT_FILENO, ">", 1);
		memset(input, 0x00, 12);
		cmd_size = read(STDIN_FILENO, input, 12);

		// Check no char
		if (cmd_size == 1)
			continue;

		// Generate argc / argv.
		if (strtotab(&argc, &argv, input) != 0) {
			printf("parser: internal error\n");
			continue;
		}

		// Check buit-in.
		if (check_builtin(argc, argv) != 0 &&
				manual_proc_call(argv) != 0)
			printf("%s: command not found\n", argv[0]);

		// Free argc /argv
		// TODO: way better way
		strtotab_quit(&argc, &argv);
	}
	return (0);
}
