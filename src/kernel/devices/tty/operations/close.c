#include <kernel/devices/tty.h>
#include <kernel/memory.h>

int tty_close(void *inode)
{
	struct tty_s *tty;
	int line;

	// Get tty object
	tty = inode;

	// Free'd all allocated memory for the
	// output buffer
	line = tty->cursor.max.y;
	while (line >= 0)
		pm_free(tty->buffers.output[line]);
	pm_free(tty->buffers.output);

	// Free'd tty object
	pm_free(tty);
	return (0);
}
