#include <lib/display.h>
#include <lib/unistd.h>
#include "util.h"

// @note
// 	For now all kernel invocation is
// made by the `unistd` library.

//TODO: documentation.
int main(void)
{
	char input[12];
	int cmd_size;
	//char **argv;
	//int argc;
	int fd;

	// Try to open TTY
	// @note:
	// 	We use O_DIRECT to avoid cache
	// generation because we do not have a
	// lot of memory.
	fd = open("/dev/tty", O_DIRECT);
	if (fd < 0)
	{
		// Display error.
		display_t disp;
		dopen(&disp, "default");
		dclear(&disp);
		dprint(&disp, 0, 0, "User program fail to open TTY");
		dprint(&disp, 0, 1, "Wait user manual reset...");
		dupdate(&disp);

		// Wait user manual reset.
		while (1)
		{
			// @note: we can use "sleep" because we
			// are in privilegied mode.
			// TODO: use sleep syscall !
			__asm__ volatile ("sleep");
		}
	}
	
	// Shell main loop.
	write(fd, "Boot Complete !\n", 16);
	while (1)
	{
		// Get user command.
		write(fd, ">", 1);
		cmd_size = read(fd, input, 12);

		// Remove '\n' char.
		// FIXME: create argc, argv !!
		input[cmd_size - 1] = '\0';

		// Check buit-in.
		//if (check_builtin(input) != 0)
		//{
			write(fd, input, cmd_size - 1);
			write(fd, ": command not found\n", 20);
		//}
	}
	return (0);
}
