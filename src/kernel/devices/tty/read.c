#include <kernel/devices/tty.h>
#include <kernel/devices/keyboard.h>
#include <kernel/context.h>
#include <kernel/atomic.h>
#include <kernel/syscall.h>
#include <lib/display.h>
#include <lib/string.h>
#include <lib/stdio.h>
#include <lib/timer.h>

// Intenral functions
static void wait_keyboard_event(void);
static int check_special(struct keyboard_obj_s *keyboard);
static int check_signal(struct keyboard_obj_s *keyboard);
static void tty_buffer_display(struct keyboard_obj_s *keyboard);
static void update_buffer(struct keyboard_obj_s *keyboard);
static int buffer_insert(struct keyboard_obj_s *keyboard, char n);
static void cursor_callback(struct keyboard_obj_s *keyboard);

//FIXME: this function is device-specifique !!
ssize_t tty_read(void *buffer, size_t count)
{
	struct keyboard_obj_s keyboard;
	extern struct tty_s tty;
	int timer_fd;

	// Check potential error.
	if (count < 2)
		return (0);

	// Initialize internal struc.
	memset(buffer, '\0', count);
	keyboard.buffer.addr = buffer;
	keyboard.buffer.size = count;
	keyboard.buffer.cursor = 0;
	keyboard.buffer.clen = 0;
	keyboard.mode = 0x00;
	keyboard.cvisible = 0;

	// save TTY informations.
	keyboard.saved.tty.cursor.x = tty.cursor.x;
	keyboard.saved.tty.cursor.y = tty.cursor.y;

	// Initialize timer for cursor.
	timer_fd = timer_install(&cursor_callback, &keyboard, 500, 1);

	// Main Loop, wait [EXE] key. 
	while ((keyboard.mode & 0x04) == 0)
 	{
		// Wait user interruption.
		wait_keyboard_event();

		// Start atomic operations and check
		// signal, special key then update
		// the buffer.
		atomic_start();
		if (check_signal(&keyboard) == 0 &&
			check_special(&keyboard) == 0)
		{
			update_buffer(&keyboard);
		}
		atomic_end();

		// Display buffer on TTY.
		tty_buffer_display(&keyboard);
	}

	// uninstall cursor timer.
	timer_uninstall(timer_fd);
	
	// Return the number of input char.
	return (keyboard.buffer.clen);
}

static void wait_keyboard_event(void)
{
	extern volatile uint8_t keylist_isUpdate;

	// Wait key list update.
	// @note:
	// 	To avoid reentrace data corruption
	// we should wait kernel indication.
	// TODO: explain correctly x)
	while (keylist_isUpdate == 0)
	{
		__asm__ volatile ("sleep");
	}
	keylist_isUpdate = 0;
}


static int check_signal(struct keyboard_obj_s *keyboard)
{
	//TODO
	(void)keyboard;
	return (0);
}

static int check_special(struct keyboard_obj_s *keyboard)
{
	extern fx9860_context_t casio_context;
	extern fx9860_context_t vhex_context;
	extern struct keycache_s *keylist;
	struct keycache_s *keynode;
	unsigned int key;

	// Find key code.
	keynode = keylist;
	while (keynode != NULL)
	{
		// Check MAJ.
		if (keynode->keycode == KEY_ALPHA && keynode->counter == 1)
			keyboard->mode = keyboard->mode ^ 0x02;

		// Check Alpha / num mode. 
		if (keynode->keycode == KEY_SHIFT && keynode->counter == 1)
			keyboard->mode = keyboard->mode ^ 0x01;

		// Check DEL key.
		if (keynode->keycode == KEY_DEL && keyboard->buffer.cursor > 0)
		{
			// Move seconde part.
			memcpy(
				&keyboard->buffer.addr[keyboard->buffer.cursor - 1],
				&keyboard->buffer.addr[keyboard->buffer.cursor],
				keyboard->buffer.clen - keyboard->buffer.cursor
			);

			// Add null char and update clen.
			keyboard->buffer.clen = keyboard->buffer.clen - 1;
			keyboard->buffer.addr[keyboard->buffer.clen] = '\0';
			keyboard->buffer.cursor = keyboard->buffer.cursor - 1;
		}

		// Check MENU key.
		if (keynode->keycode == KEY_MENU &&  keynode->counter == 1)
		{
			// Save current Vhex context and restore Casio's context.
			atomic_start();
			fx9860_context_save(&vhex_context);
			fx9860_context_restore(&casio_context);
			atomic_end();

			// Inject MENU key and call GetKey().
			// TODO !!!
			//int row = 0;
			//int column = 0;
			//uint16_t keymatrix = 0x0308;
			//casio_Bkey_PutKeymatrix(&keymatrix);
			//casio_GetKeyWait(&row, &column, 0, 0, 0, &key);
			casio_GetKey(&key);
			
			// Save current Casio's context and restore Vhex's context.
			atomic_start();
			fx9860_context_save(&casio_context);
			fx9860_context_restore(&vhex_context);
			atomic_end();
		}

		// Check EXE key.
		if (keynode->keycode == KEY_EXE && keynode->counter == 1)
		{
			// Add new line character.
			keyboard->buffer.addr[keyboard->buffer.clen] = '\n';
			keyboard->buffer.clen = keyboard->buffer.clen + 1;
			keyboard->buffer.addr[keyboard->buffer.clen] = '\0';
			
			// indicate that the EXE has been pressed.
			keyboard->mode = keyboard->mode | 0x04;
			return (1);
		}

		// Check LEFT key.
		if (keynode->keycode == KEY_LEFT &&
				keynode->counter == 1 &&
				keyboard->buffer.cursor > 0)
		{
			keyboard->buffer.cursor = keyboard->buffer.cursor - 1;
		}

		// Check RIGHT key.
		if (keynode->keycode == KEY_RIGHT &&
				keynode->counter == 1 &&
				keyboard->buffer.cursor < keyboard->buffer.clen)
		{
			keyboard->buffer.cursor = keyboard->buffer.cursor + 1;
		}

		//TODO: AC/on
		// get next keynode
		keynode = keynode->next;
	}
	return (0);
}

static void tty_buffer_display(struct keyboard_obj_s *keyboard)
{
	extern struct tty_s tty;
	size_t size;

	// Restore TTY X/Y axis positions.
	tty.cursor.x = keyboard->saved.tty.cursor.x;
	tty.cursor.y = keyboard->saved.tty.cursor.y;

	// Workaround for [EXE] key.
	size =
		((keyboard->mode & 0x04) == 0)
		? keyboard->buffer.clen + 1
		: keyboard->buffer.clen;


	// Write buffer.
	tty_write(keyboard->buffer.addr, size);

	// Check Y-axis udpate.
	// TODO !!!
	//if (keyboard->saved.tty.cursor.y < tty.cursor.y)
	//	keyboard->saved.tty.cursor.y -=
	//		tty.cursor.y - keyboard->saved.tty.cursor.y;
}

static void update_buffer(struct keyboard_obj_s *keyboard)
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
		KEY_DOT, KEY_FD, KEY_UNUSED
	};
	static const char keylist_num_char[] = "0123456789+-x/(),^.|";
	extern struct keycache_s *keylist;
	const uint8_t *keycode_list;
	struct keycache_s *keynode;
	char character;
	int i;

	// Get the appropriate key list.
	keycode_list =
		((keyboard->mode & 1) == 0)
		? keylist_alpha
		: keylist_num;

	// Walk into pressed key list
	keynode = keylist;
	while (keynode != NULL && keynode->counter == 1)
	{
		// Try to find the pressed key.
		i = -1;
		while (keycode_list[++i] != KEY_UNUSED &&
				keycode_list[i] != keynode->keycode);
		
		// If the key match, update buffer.
		if (keycode_list[i] == keynode->keycode)
		{
			// Get the char to be written
			if ((keyboard->mode & 1) == 0)
				character = (keyboard->mode & 2) ? 'A' + i : 'a' + i;
			else
				character = keylist_num_char[i];
	
			// Insert new character if possible.
			// if not, return;
			if (buffer_insert(keyboard, character) != 0)
				return;
		}

		// Get the next pressed key.
		keynode = keynode->next;
	}
}

static int buffer_insert(struct keyboard_obj_s *keyboard, char n)
{
	// Check if the buffer is full.
	if (keyboard->buffer.clen + 1 >= keyboard->buffer.size)
		return (1);

	// Move the seconds par of the buffer is needed
	if (keyboard->buffer.cursor < keyboard->buffer.clen)
	{
		int i = keyboard->buffer.clen + 1;
		while (--i >= (int)keyboard->buffer.cursor)
			keyboard->buffer.addr[i] = keyboard->buffer.addr[i - 1];
	}

	// Add the new character.
	keyboard->buffer.addr[keyboard->buffer.cursor] = n;

	// Update cursor and character len.
	keyboard->buffer.cursor = keyboard->buffer.cursor + 1;
	keyboard->buffer.clen = keyboard->buffer.clen + 1;
	return (0);
}

static void cursor_callback(struct keyboard_obj_s *keyboard)
{
	extern struct tty_s tty;
	int x;
	int y;

	// Draw cursor if needed
	if (keyboard->cvisible == 0)
	{
		x = keyboard->buffer.cursor + keyboard->saved.tty.cursor.x;
		y = x / tty.cursor.max.x;
		x = x - (y * tty.cursor.max.x);
		y = y + keyboard->saved.tty.cursor.y;
		dreverse(
			x * (KERNEL_FONT_REAL_WIDTH + 1),
			y * (KERNEL_FONT_REAL_HEIGHT + 1),
			(KERNEL_FONT_REAL_WIDTH + 1),
			(KERNEL_FONT_REAL_HEIGHT + 1)
		);
		dupdate();
	}
	// Update cursor status.
	keyboard->cvisible = keyboard->cvisible ^ 1;
}
