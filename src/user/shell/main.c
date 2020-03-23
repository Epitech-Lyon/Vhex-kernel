#include <lib/display.h>
#include <lib/unistd.h>
#include "util.h"

//TODO: documentation.
int main(void)
{
	char input[12];
	int cmd_size;

	// Shell main loop.
	write(STDOUT_FILENO, "Boot Complete !\n", 16);
	while (1)
	{
		// Get user command.
		write(STDOUT_FILENO, ">", 1);
		cmd_size = read(STDIN_FILENO, input, 12);

		// Check no char
		if (cmd_size == 1)
			continue;

		// Remove '\n' char.
		// FIXME: create argc, argv !!
		input[cmd_size - 1] = '\0';

		// Check buit-in.
		if (check_builtin(input) != 0)
		{
			write(STDOUT_FILENO, input, cmd_size - 1);
			write(STDOUT_FILENO, ": command not found\n", 20);
		} 
	}
	return (0);
}
