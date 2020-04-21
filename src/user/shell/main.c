#include <unistd.h>
#include <string.h>
#include <signal.h>
#include <stdio.h>
#include "util.h"

//TODO: documentation.
int main(void)
{
	char input[12];
	int cmd_size;
	char **argv;
	int argc;

	// Shell main loop.
	write(STDOUT_FILENO, "Boot Complete !\n", 16);

	// Ignore internal signals (keyboard)
	//signal(SIGINT, SIG_IGN);
	//signal(SIGQUIT, SIG_IGN);
	//signal(SIGTSTP, SIG_IGN);

	// Main loop
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
