#include <lib/display.h>
#include "util.h"

//TODO: documentation.
int main(void)
{
	dclear();
	dprint(0, 0, "First user program !");
	dupdate();
	while (1);


/*	char input[12];
	int cmd_size;
	char **argv;
	int argc;

	// Open TTY.
	tty_open();
	
	// Entry ! :D
	tty_write("Boot Complete !\n", 16);

	// Shell main loop.
	while (1)
	{
		// Get user command.
		tty_write(">", 1);
		cmd_size = tty_read(input, 12);

		// Remove '\n' char.
		// FIXME: create argc, argv !!
		input[cmd_size - 1] = '\0';

		// Check buit-in.
		if (check_builtin(input) != 0)
		{
			tty_write(input, cmd_size - 1);
			tty_write(": command not found\n", 20);
		}
	}
	return (0);*/
}
