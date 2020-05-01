#include <kernel/devices/tty.h>
#include <kernel/devices/earlyterm.h>
#include <kernel/drivers/keyboard.h>
#include <kernel/drivers/timer.h>
#include <kernel/util/atomic.h>
#include <kernel/context.h>
#include <kernel/syscall.h>
#include <kernel/signal.h>
#include <sys/signal.h>
#include <pthread.h>
#include <string.h>

static void tty_init_internal(struct tty *tty)
{
	// Wait TTY keyboard availability
	pthread_mutex_lock(&tty->keyboard.private.mutex);

	// Wait TTY screen availability
	pthread_mutex_lock(&tty->screen.private.mutex);

	// save TTY informations.
	tty->keyboard.saved.tty.cursor.x = tty->screen.cursor.disp.x;
	tty->keyboard.saved.tty.cursor.y = tty->screen.cursor.disp.y;

	// Unlock the TTY screen
	pthread_mutex_unlock(&tty->screen.private.mutex);

	// Start timer (if needed) for cursor blink.
	if (tty->keyboard.private.timer.used == 0)
		timer_start(tty->keyboard.private.timer.id);
	tty->keyboard.private.timer.used += 1;

	// Unlock device
	pthread_mutex_unlock(&tty->keyboard.private.mutex);
}

//FIXME: user mutex and remove atomic_* !!
//FIXME: with mutex use DMA transfert insted of memset() !
static ssize_t tty_uninit_internal(struct tty *tty, void *buffer, size_t count)
{
	// Wait device availability
	pthread_mutex_lock(&tty->keyboard.private.mutex);

	// Get the real size to dump
	if (count > tty->keyboard.private.cursor_max)
		count = tty->keyboard.private.cursor_max;

	// Dump internal buffer
	memcpy(buffer, tty->buffers.input, count);

	// Update the input buffer if needed
	if (count < tty->keyboard.private.cursor_max) {
		memcpy(tty->buffers.input, &tty->buffers.input[count],
				tty->keyboard.private.cursor_max - count);
	}

	// Update internal cursor
	tty->keyboard.private.cursor_max = 0;
	tty->keyboard.private.cursor = 0;

	// Stop the cursor timer if needed.
	tty->keyboard.private.timer.used -= 1;
	if (tty->keyboard.private.timer.used == 0)
		timer_stop(tty->keyboard.private.timer.id);
	
	// Unlock device
	pthread_mutex_unlock(&tty->keyboard.private.mutex);

	// Return the number of input char.
	return (count);
}

//FIXME: user mutex and remove atomic_* !!
ssize_t tty_read(void *inode, void *buffer, size_t count)
{
	keyscan_t keylist[KEYBOARD_NB_KEYS];
	struct tty *tty;

	// Check potential error.
	if (count == 0)
		return (0);
	
	// Get tty devie
	tty = inode;

	// Intialize internal struct
	tty_init_internal(tty);

	// Main Loop, wait [EXE] key. 
	// TODO: use thread !!!
 	do {
		// Wait user interactions
		keyboard_wait_event(keylist);

		// Wait device availability
		pthread_mutex_lock(&tty->keyboard.private.mutex);

		// Check the keys list
		tty->keyboard.mode.ctrl = 0;
		tty->keyboard.mode.enter = 0;
		tty->keyboard.saved.signal.keycode = KEY_UNUSED;
		for (int i = 0 ; keylist[i].keycode != KEY_UNUSED && i < KEYBOARD_NB_KEYS ; ++i)
		{
			// Check key validity
			if (!(keylist[i].keycode == KEY_OPTN || keylist[i].counter == 1
					|| (i == 0 && keylist[i].counter > 10 && (keylist[i].counter & 1) == 0)))
				continue;

			// Handle current key
			if (tty_keyboard_check_special(tty, keylist[i].keycode) == 0)
				tty_keyboard_buffer_update(tty, keylist[i].keycode);

			// Dump the the fist pressed key
			// @note: this is a workaround for the signal management
			if (i == 0) {
				tty->keyboard.saved.signal.keycode = keylist[i].keycode;
				tty->keyboard.saved.signal.cursor = tty->keyboard.private.cursor;
			}
		}

		// Unlock device
		pthread_mutex_unlock(&tty->keyboard.private.mutex);

		// Check signal
		tty_keyboard_check_signal(tty);

		// Display intput buffer on TTY.
		tty_keyboard_buffer_echoing(tty);
	} while (tty->keyboard.mode.enter == 0);
	return (tty_uninit_internal(tty, buffer, count));
}
