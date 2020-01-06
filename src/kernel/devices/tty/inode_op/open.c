#include <kernel/devices/tty.h>
#include <kernel/devices/display.h>
#include <kernel/devices/keyboard.h>
#include <kernel/util.h>

// Internal TTY object.
struct tty_s tty;

void *tty_open(dev_t major, dev_t minor)
{
	int lines;

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

	// Set TTY primitives
	// TODO: Add USB primitives
	// TODO: update this part.
	tty.primitives.write = &display_write;
	tty.primitives.read = &keyboard_read;

	// Call TTY primitives constructor
	keyboard_open();
	display_open();
	return (&tty);
}
