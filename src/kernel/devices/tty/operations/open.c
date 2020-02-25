#include <kernel/devices/tty.h>
#include <kernel/util/string.h>

// Internal TTY object.
struct tty_s tty;

//TODO: MULTIPLE OPEN !!!!
//TODO: SHARED TTY DEVICE !!!!
void *tty_open(dev_t major, dev_t minor)
{
	int lines;

	// TODO: handle major / minor ?
	(void)major;
	(void)minor;

	// Initialize TTY cursor.
	tty.cursor.x = 0;
	tty.cursor.y = 0;
	tty.cursor.max.x = TTY_BUFFER_COLUMNS;
	tty.cursor.max.y = TTY_BUFFER_LINES;

	// Initialize TTY buffer.
	// TODO: user can set dynamicaly the TTY buffer size ?
	lines = TTY_BUFFER_LINES;
	while (--lines >= 0)
		memset(tty.buffer[lines], '\0', TTY_BUFFER_COLUMNS);
	return (&tty);
}
