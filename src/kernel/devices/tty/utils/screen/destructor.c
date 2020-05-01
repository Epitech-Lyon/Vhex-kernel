#include <kernel/devices/tty.h>
#include <kernel/drivers/timer.h>

void tty_screen_destructor(struct tty *tty)
{
	// Uninstall hardware timer 
	if (tty->screen.private.timer.id != -1)
		timer_uninstall(tty->screen.private.timer.id);

	// Destroy internal mutex
	pthread_mutex_destroy(&tty->screen.private.mutex);
}
