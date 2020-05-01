#include <kernel/devices/tty.h>
#include <kernel/devices/earlyterm.h>
#include <kernel/drivers/screen.h>
#include <kernel/drivers/timer.h>
#include <kernel/util/atomic.h>
#include <display.h>

/* tty_screen_display_callback() - flush ony characte of the output buffer */
void tty_screen_display_callback(struct tty *tty)
{
	int update;

	// Force atomic operation
	atomic_start();

	// Check useless action
	update = 0;
	while (tty->screen.cursor.flush != tty->screen.cursor.write)
	{
		tty_screen_discipline_char(tty, tty->buffers.output[tty->screen.cursor.flush], 1);
		tty->screen.cursor.flush = tty->screen.cursor.flush + 1;
		if (tty->screen.cursor.flush >= TTY_OUTPUT_BUFFSIZE)
			tty->screen.cursor.flush = 0;
		update = 1;
	}

	// Update screen if needed
	if (update == 1)
		screen_update(tty->screen.private.disp.vram);

	// Stop timer if needed
	if (tty->screen.private.timer.status == 0) {
		timer_stop(tty->screen.private.timer.id);
		pthread_mutex_unlock(&tty->screen.private.mutex);
	}

	// Stop atomic operation
	atomic_stop();
}
