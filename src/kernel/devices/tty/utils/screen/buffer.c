#include <kernel/devices/tty.h>
#include <kernel/devices/earlyterm.h>
#include <kernel/drivers/timer.h>
#include <kernel/util/atomic.h>
#include <string.h>

/*
** tty_screen_buffer_flush() - Force display all character
** currently in the output buffer
*/
void tty_screen_buffer_flush(struct tty *tty)
{
	// start atomic operations
	atomic_start();

	// Force flush
	while (tty->screen.cursor.flush != tty->screen.cursor.write)
		tty_screen_display_callback(tty);

	// Stop atomic operations
	atomic_stop();
}

/* Wait flush syncronisation (wait flush timer) */
static void wait_sync_flush(struct tty *tty)
{
	// Wait syncro
	while (1) {
		// Check syncro
		if (tty->screen.cursor.flush == tty->screen.cursor.write)
			return;

		// Wait schedule
		// FIXME: force schedule and preemptive !!
		__asm__ volatile ("sleep");
	}
}

// TODO: use DMA !!!!!!
ssize_t tty_screen_buffer_update(struct tty *tty, const uint8_t *buffer, size_t count)
{
	size_t transaction_size;
	size_t size;
	int counter;
	int again;

	// Check useless action
	if (count == 0)
		return (0);

	// Wait TTY screen part
	pthread_mutex_lock(&tty->screen.private.mutex);

	// Start flush timer
	again = tty->screen.private.timer.status;
	tty->screen.private.timer.status += 1;
	if (again == 0)
		timer_start(tty->screen.private.timer.id);

	// Send buffer data in the output buffer
	size = 0;
	counter = 0;
	transaction_size = 0;
	while (size < count)
	{
		// Get the transaction size
		transaction_size = (count - size) / (TTY_OUTPUT_BUFFSIZE / 2);
		if (transaction_size == 0)
			transaction_size = count - size;
		else
			transaction_size = TTY_OUTPUT_BUFFSIZE / 2;

		// Check output buffer limit
		if (tty->screen.cursor.write + transaction_size >= TTY_OUTPUT_BUFFSIZE)
			transaction_size = TTY_OUTPUT_BUFFSIZE - tty->screen.cursor.write;

		// Wait output buffer syncro and send data
		wait_sync_flush(tty);
		memcpy(&tty->buffers.output[tty->screen.cursor.write], &buffer[size], transaction_size);

		// Update current written size
		size = size + transaction_size;

		// Update buffer write cursor
		tty->screen.cursor.write += transaction_size;
		if (tty->screen.cursor.write >= TTY_OUTPUT_BUFFSIZE)
			tty->screen.cursor.write -= TTY_OUTPUT_BUFFSIZE;
	}

	// Indicate that we have finish to write
	tty->screen.private.timer.status -= 1;
	return (count);
}
