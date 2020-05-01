#include <kernel/devices/tty.h>
#include <kernel/drivers/timer.h>

void tty_keyboard_destructor(struct tty *tty)
{
	// Uninstall hardware timer 
	if (tty->keyboard.private.timer.id != -1)
		timer_uninstall(tty->keyboard.private.timer.id);

	// Destroy internal mutex
	pthread_mutex_destroy(&tty->keyboard.private.mutex);
}
