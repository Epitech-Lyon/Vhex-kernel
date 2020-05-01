#include <kernel/devices/tty.h>
#include <kernel/drivers/timer.h>
#include <kernel/util/atomic.h>
#include <kernel/util/kmem.h>

int tty_close(void *inode)
{
	struct tty *tty;

	// Get tty object
	tty = inode;

	// Force flush internal buffers
	tty_keyboard_buffer_flush(tty);
	tty_screen_buffer_flush(tty);

	// Start atomic operations
	atomic_start();

	// Call each module destructors
	tty_keyboard_destructor(tty);
	tty_screen_destructor(tty);

	// Stop atomic operations
	atomic_stop();
	return (0);
}
