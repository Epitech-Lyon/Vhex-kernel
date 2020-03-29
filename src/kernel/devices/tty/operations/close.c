#include <kernel/devices/tty.h>
#include <kernel/util/kmem.h>

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
		kmem_free(tty->buffers.output[line]);
	kmem_free(tty->buffers.output);

	// Free'd tty object
	kmem_free(tty);
	return (0);
}
