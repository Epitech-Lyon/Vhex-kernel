#include <kernel/devices/tty.h>
#include <kernel/devices/earlyterm.h>
#include <kernel/drivers/screen.h>
#include <kernel/drivers/timer.h>
#include <kernel/util/kmem.h>
#include <string.h>

int tty_screen_constructor(struct tty *tty)
{
	// Try to get the font used by the tty.
	// @note: we use drawing lib primitives
	if (dopen(&tty->screen.private.disp, "default") != 0)
		return (-1);

	// Try to get hardware timer slot
	tty->screen.private.timer.id = timer_install(&tty_screen_display_callback, tty, 16, TIMER_STOP);
	if (tty->screen.private.timer.id < 0) {
		earlyterm_write("TTY: screen flush timer error\n");
		return (-1);
	}

	// Generate "visible" informations for the write primitive
	tty->screen.winsize.ws_xpixel = screen_get(SCREEN_WIDTH);
	tty->screen.winsize.ws_ypixel = screen_get(SCREEN_HEIGHT);
	tty->screen.winsize.ws_col = tty->screen.winsize.ws_xpixel / (tty->screen.private.disp.font->font.width + 1);
	tty->screen.winsize.ws_row = tty->screen.winsize.ws_ypixel / (tty->screen.private.disp.font->font.height + 1);

	// Initialize default TTY screen abstraction cursor position.
	tty->screen.cursor.disp.x = 0;
	tty->screen.cursor.disp.y = 0;

	// Intializei buffer cursor
	tty->screen.cursor.flush = 0;
	tty->screen.cursor.write = 0;
	return (0);
}
