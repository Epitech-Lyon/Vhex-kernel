#include <kernel/devices/tty.h>
#include <kernel/drivers/screen.h>
#include <kernel/util/atomic.h>

void tty_cursor_callback(volatile struct tty *tty)
{
	static int cursor_visible = 0;
	int saved_tty_x;
	int saved_tty_y;
	int x;
	int y;

	// Force atomic operation
	atomic_start();

	// Draw cursor if needed
	if (cursor_visible == 0)
	{
		// Save current TTY cursor positions
		saved_tty_x = tty->screen.cursor.disp.x;
		saved_tty_y = tty->screen.cursor.disp.y;

		// Set "real" position
		tty->screen.cursor.disp.x = tty->keyboard.saved.tty.cursor.x;
		tty->screen.cursor.disp.y = tty->keyboard.saved.tty.cursor.y;

		// Walk into input buffer and simulate the character
		// echoing to force the internal line discipline to
		// update TTY cursor position
		for (off_t i = 0 ; i < tty->keyboard.private.cursor ; ++i)
			tty_screen_discipline_char((void*)tty, tty->buffers.input[i], 0);

		// Get "real" X and Y position (pixel)
		x = tty->screen.cursor.disp.x * (tty->screen.private.disp.font->font.width + 1);
		y = tty->screen.cursor.disp.y * (tty->screen.private.disp.font->font.height + 1);
		
		// Draw and display cursor.
		dreverse(
			(void *)&tty->screen.private.disp, x, y,
			(tty->screen.private.disp.font->font.width + 1),
			(tty->screen.private.disp.font->font.height + 1)
		);
		screen_update((void *)tty->screen.private.disp.vram);

		// Restore old TTY position
		tty->screen.cursor.disp.x = saved_tty_x;
		tty->screen.cursor.disp.y = saved_tty_y;
	}
	
	// Update cursor status.
	cursor_visible ^= 1;

	// Stop atomic operations
	atomic_stop();
}
