#include <kernel/devices/tty.h>
#include <kernel/devices/earlyterm.h>
#include <kernel/drivers/screen.h>
#include <kernel/util/atomic.h>
#include <lib/pthread.h>
#include <lib/display.h>
#include <lib/string.h>

/* tty_keyboard_buffer_flush() - force flush the internal buffer */
void tty_keyboard_buffer_flush(struct tty *tty)
{
	// Start atomic operation
	atomic_start();

	// Force echo the internal buffer
	tty_keyboard_buffer_echoing(tty);

	tty->keyboard.private.cursor = 0;
	tty->keyboard.private.cursor_max = 0;

	// Stop atomic operations
	atomic_stop();
}

/* 
** tty_buffer_remove() - Remove the character based on current
** cursor position
** @note: this function should be called ONLY if the device
** is locked
*/
// TODO: use DMA ?
void tty_keyboard_buffer_remove(struct tty *tty, off_t cursor)
{
	if (cursor <= 0 || cursor > tty->keyboard.private.cursor_max) {
		pthread_mutex_unlock(&tty->keyboard.private.mutex);
		return;
	}

	// Remove target char using areas overlaps (if needed)
	if (cursor < tty->keyboard.private.cursor_max)
	{
		memcpy(
			&tty->buffers.input[cursor - 1],
			&tty->buffers.input[cursor],
			tty->keyboard.private.cursor_max - cursor
		);
	}

	// Update keyboard cu
	tty->keyboard.private.cursor_max = tty->keyboard.private.cursor_max - 1;
	tty->keyboard.private.cursor = tty->keyboard.private.cursor - 1;
}

/* 
** tty_buffer_insert() - Insert new character based on current
** cursor position
** @note: this function should be called ONLY if the device
** is locked
*/
int tty_keyboard_buffer_insert(struct tty *tty, char n)
{
	// Check if the buffer is full.
	if (tty->keyboard.private.cursor_max + 1 >= TTY_INPUT_BUFFSIZE)
		return (1);

	// Move the seconds par of the buffer is needed
	// TODO: use DMA ?
	if (tty->keyboard.private.cursor < tty->keyboard.private.cursor_max)
	{
		int i = tty->keyboard.private.cursor_max + 1;
		while (--i >= (int)tty->keyboard.private.cursor)
			tty->buffers.input[i] = tty->buffers.input[i - 1];
	}

	// Add the new character.
	tty->buffers.input[tty->keyboard.private.cursor] = n;

	// Update cursor and character len.
	tty->keyboard.private.cursor = tty->keyboard.private.cursor + 1;
	tty->keyboard.private.cursor_max = tty->keyboard.private.cursor_max + 1;
	return (0);
}

/* tty_keyboard_buffer_echoing() - Display the internal buffer */
void tty_keyboard_buffer_echoing(struct tty *tty)
{
	// Start atomic operation
	atomic_start();

	// Save current TTY cursor positions
	//saved_tty_x = tty->screen.cursor.disp.x;
	//saved_tty_y = tty->screen.cursor.disp.y;

	// Set "real" position
	tty->screen.cursor.disp.x = tty->keyboard.saved.tty.cursor.x;
	tty->screen.cursor.disp.y = tty->keyboard.saved.tty.cursor.y;

	// Walk into input buffer and simulate the character
	// echoing to force the internal line discipline to
	// update TTY cursor position
	for (off_t i = 0 ; i < tty->keyboard.private.cursor_max ; ++i)
	{
		// Clear the area
		drect(&tty->screen.private.disp,
			tty->screen.cursor.disp.x * (tty->screen.private.disp.font->font.width + 1),
			tty->screen.cursor.disp.y * (tty->screen.private.disp.font->font.height + 1),
			tty->screen.private.disp.font->font.width + 1,
			tty->screen.private.disp.font->font.height + 1);

		// Update cursor and display char if needed
		tty_screen_discipline_char((void*)tty, tty->buffers.input[i], 1);
	}

	// workaround for the DEL actions
	drect(&tty->screen.private.disp,
		tty->screen.cursor.disp.x * (tty->screen.private.disp.font->font.width + 1),
		tty->screen.cursor.disp.y * (tty->screen.private.disp.font->font.height + 1),
		tty->screen.private.disp.font->font.width + 1,
		tty->screen.private.disp.font->font.height + 1);

	// Update screen
	screen_update((void *)tty->screen.private.disp.vram);

	// Stop atomic operation
	atomic_stop();
}

/* 
** tty_keyboard_buffer_update() - Udate the internal buffer
** with the given key code
** @note: this function should be called ONLY if the device
** is locked
*/
int tty_keyboard_buffer_update(struct tty *tty, key_t key)
{
	static const uint8_t keylist_alpha[] = {
		KEY_XOT, KEY_LOG, KEY_LN, KEY_SIN, KEY_COS, KEY_TAN,
		KEY_FRAC, KEY_FD, KEY_LEFTP, KEY_RIGHTP, KEY_COMMA, KEY_ARROW,
		KEY_7, KEY_8, KEY_9,
		KEY_4, KEY_5, KEY_6, KEY_MUL, KEY_DIV,
		KEY_1, KEY_2, KEY_3, KEY_PLUS, KEY_MINUS,
		KEY_0, KEY_UNUSED
	};
	static const uint8_t keylist_num[] = {
		KEY_0, KEY_1, KEY_2, KEY_3, KEY_4,
		KEY_5, KEY_6, KEY_7, KEY_8, KEY_9,
		KEY_PLUS, KEY_MINUS, KEY_MUL, KEY_DIV,
		KEY_LEFTP, KEY_RIGHTP, KEY_COMMA, KEY_POWER,
		KEY_DOT, KEY_FD, KEY_ARROW, KEY_UNUSED
	};
	static const char keylist_num_char[] = "0123456789+-x/(),^.|_";
	const uint8_t *keycode_list;
	char character;
	int i;

	// Get the appropriate key list.
	keycode_list = keylist_alpha;
	if (tty->keyboard.mode.special == 1)
		keycode_list = keylist_num;

	// Try to find the pressed key.
	i = -1;
	while (keycode_list[++i] != KEY_UNUSED && keycode_list[i] != key);
	if (keycode_list[i] != key)
		return (0);

	// If the key match, update buffer and get the
	// character to be written
	character = keylist_num_char[i];
	if (tty->keyboard.mode.special == 0) {
		character =  'a' + i;
		if (tty->keyboard.mode.maj == 1)
			character = 'A' + i;;
	}
	
	// Insert new character if possible.
	tty_keyboard_buffer_insert(tty, character);
	return (1);
}
