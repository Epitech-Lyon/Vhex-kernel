#include <kernel/devices/tty.h>
#include <kernel/drivers/screen.h>
#include <kernel/util/atomic.h>

/* tty_write() - TTY write primitive ! */
ssize_t tty_write(void *inode, const void *buffer, size_t count)
{
	struct tty *tty;

	// Get tty device
	tty = inode;

	// Start atomic operation
	atomic_start();

	// Check STDERR ouput mode (force print)
	// FIXME: move me !!!!
	if (tty->screen.private.nocache == 1) {
		size_t i = -1;
		while (++i < count)
			tty_screen_discipline_char(tty, ((char*)buffer)[i], 1);
		screen_update(tty->screen.private.disp.vram);
		tty->screen.private.nocache = 0;
		atomic_stop();
		return (i);
	}

	// Stop atomic operation
	atomic_stop();

	// Update internal output buffer
	// @note: it will be displayed monotoinically using
	// hardware timer (~16hz)
	return (tty_screen_buffer_update(inode, buffer, count));
}
