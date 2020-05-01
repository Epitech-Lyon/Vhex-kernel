#include <kernel/devices/tty.h>
#include <kernel/devices/earlyterm.h>
#include <kernel/util/atomic.h>
#include <kernel/util/kmem.h>
#include <string.h>

// TTY device
struct tty tty;

static void tty_default_init(struct tty *tty)
{
	// Wipe all object data
	memset(tty, 0x00, sizeof(struct tty));

	// Force default value
	// TODO: move to "./utils/<module>" appropriate part ?
	tty->keyboard.private.timer.id = -1;
	tty->screen.private.timer.id = -1;
}

void *tty_open(dev_t major, dev_t minor)
{
	// TODO: handle major / minor !!
	(void)major;
	(void)minor;
	
	// Start atomic operations
	atomic_start();

	// Initialize TTY
	tty_default_init(&tty);
	if (tty_screen_constructor(&tty) != 0
			|| tty_keyboard_constructor(&tty) != 0)
	{
		tty_close(&tty);
		atomic_stop();
		return (NULL);
	}

	// Stop atomic operations and return the device
	atomic_stop();
	return (&tty);
}
