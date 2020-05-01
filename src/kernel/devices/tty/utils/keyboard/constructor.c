#include <kernel/devices/tty.h>
#include <kernel/drivers/timer.h>
#include <kernel/devices/earlyterm.h>
#include <kernel/util/atomic.h>
#include <pthread.h>

int tty_keyboard_constructor(struct tty *tty)
{
	extern void tty_cursor_callback(volatile struct tty *tty);

	// Start atomic
	atomic_start();

	// Try to get hardware timer slot
	tty->keyboard.private.timer.id = timer_install(&tty_cursor_callback, tty, 2, TIMER_STOP);
	if (tty->keyboard.private.timer.id < 0) {
		earlyterm_write("TTY: keyboard cursor's timer error\n");
		return (0);
	}

	// Intitialise internal data used by the
	// abstraction primitive
	tty->keyboard.private.cursor = 0;
	tty->keyboard.private.cursor_max = 0;

	// Initialize internal mutex
	pthread_mutex_init(&tty->keyboard.private.mutex, NULL);

	// Intialize internal keyboard mode
	tty->keyboard.mode.enter = 0;
	tty->keyboard.mode.maj = 0;
	tty->keyboard.mode.special = 0;

	// Start atomic
	atomic_stop();
	return (0);
}
