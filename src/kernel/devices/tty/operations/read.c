#include <kernel/devices/tty.h>
#include <kernel/drivers/keyboard.h>
#include <kernel/drivers/timer.h>
#include <kernel/util/atomic.h>
#include <kernel/util/debug.h>
#include <kernel/util/string.h>
#include <kernel/util/casio.h>
#include <kernel/context.h>
#include <kernel/syscall.h>

// Intenral functions
static void wait_keyboard_event(void);
static int check_special(struct keyboard_obj_s *keyboard, key_t key);
static int check_signal(struct keyboard_obj_s *keyboard, key_t key);
static void tty_buffer_display(struct keyboard_obj_s *keyboard);
static int update_buffer(struct keyboard_obj_s *keyboard, key_t key);
static int buffer_insert(struct keyboard_obj_s *keyboard, char n);
static void cursor_callback(struct keyboard_obj_s *keyboard);

//FIXME: this function is device-specifique !!
ssize_t tty_read(void *inode, void *buffer, size_t count)
{
	extern struct keycache_s *keylist;
	struct keycache_s *keynode;
	struct keyboard_obj_s keyboard;
	int first_key;
	int timer_fd;

	// Check potential error.
	if (count < 2)
		return (0);
	
	// get tty device

	// Initialize internal struc.
	memset(buffer, '\0', count);
	keyboard.buffer.addr = buffer;
	keyboard.buffer.size = count;
	keyboard.buffer.cursor = 0;
	keyboard.buffer.clen = 0;
	keyboard.mode = 0x00;
	keyboard.cvisible = 0;
	keyboard.tty = inode;

	// save TTY informations.
	keyboard.saved.tty.cursor.x = keyboard.tty->cursor.x;
	keyboard.saved.tty.cursor.y = keyboard.tty->cursor.y;

	// Initialize timer for cursor.
	// FIXME: find real ticks value !!
	timer_fd = timer_install(&cursor_callback, &keyboard, 500 * 500 * 2, TIMER_START);
	if (timer_fd == -1)
		return (0);

	// Main Loop, wait [EXE] key. 
	while ((keyboard.mode & 0x04) == 0)
 	{
		// Wait user interruption.
		wait_keyboard_event();

		// Start atomic operations and check
		// signal, special key then update
		// the buffer.
		atomic_start();
		keynode = keylist;
		first_key = 0;
		while (keynode != NULL && (keynode->counter == 1 ||
					(first_key == 0 &&
					 keynode->counter > 10 &&
					 (keynode->counter & 1) == 0)))
		{
			// Repeat key flags.
			first_key = 1;

			// Handle current key
			if (check_signal(&keyboard, keynode->keycode) == 0 &&
					check_special(&keyboard, keynode->keycode) == 0)
				update_buffer(&keyboard, keynode->keycode);

			// Get next key.
			keynode = keynode->next;
		}
		atomic_stop();

		// Display buffer on TTY.
		tty_buffer_display(&keyboard);
	}

	// uninstall cursor timer.
	timer_uninstall(timer_fd);
	
	// Return the number of input char.
	return (keyboard.buffer.clen);
}

//FIXME: function driver specific !
static void wait_keyboard_event(void)
{
	extern volatile uint8_t keylist_isUpdate;

	// Wait key list update.
	// @note:
	// 	To ensure reentrace and avoid data corruption
	// we should wait kernel indication.
	// TODO: explain correctly x)
	while (keylist_isUpdate == 0)
	{
		__asm__ volatile ("sleep");
	}
	keylist_isUpdate = 0;
}

static int check_signal(struct keyboard_obj_s *keyboard, key_t key)
{
	//TODO
	(void)key;
	(void)keyboard;
	return (0);
}

static int check_special(struct keyboard_obj_s *keyboard, key_t key)
{
	extern fx9860_context_t casio_context;
	extern fx9860_context_t vhex_context;

	switch (key)
	{
		// Check MAJ.
		case KEY_ALPHA:
		{
			keyboard->mode = keyboard->mode ^ 0x02;
			return (1);
		}

		// Check Alpha / num mode. 
		case KEY_SHIFT:
		{
			keyboard->mode = keyboard->mode ^ 0x01;
			return (1);
		}

		// Check DEL key.
		case KEY_DEL:
		{
			// Check potential error.
			if (keyboard->buffer.cursor <= 0)
				return (1);

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
			return (1);
		}

		// Check MENU key.
		// TODO: fix me !
		case KEY_MENU:
		{
			// Save current Vhex context and restore Casio's context.
			atomic_start();
			fx9860_context_save(&vhex_context);
			fx9860_context_restore(&casio_context);
			atomic_stop();

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
			atomic_stop();
			return (1);
		}

		// Check EXE key.
		case KEY_EXE:
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
		case KEY_LEFT:
		{
			// Check potential error.
			if (keyboard->buffer.cursor <= 0)
				return (1);

			// Move cursor.
			keyboard->buffer.cursor = keyboard->buffer.cursor - 1;
			return (1);
		}

		// Check RIGHT key.
		case KEY_RIGHT:
		{
			if (keyboard->buffer.cursor >= keyboard->buffer.clen)
				return (1);
		
			// Move cursor.
			keyboard->buffer.cursor = keyboard->buffer.cursor + 1;
			return (1);
		}
		default: break;
	}
	return (0);
}

static void tty_buffer_display(struct keyboard_obj_s *keyboard)
{
	size_t size;

	// Restore TTY X/Y axis positions.
	keyboard->tty->cursor.x = keyboard->saved.tty.cursor.x;
	keyboard->tty->cursor.y = keyboard->saved.tty.cursor.y;

	// Workaround for [EXE] key.
	size =
		((keyboard->mode & 0x04) == 0)
		? keyboard->buffer.clen + 1
		: keyboard->buffer.clen;

	// Write buffer.
	tty_write(NULL, keyboard->buffer.addr, size);
}

static int update_buffer(struct keyboard_obj_s *keyboard, key_t key)
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
	keycode_list =
		((keyboard->mode & 1) == 0)
		? keylist_alpha
		: keylist_num;

	// Try to find the pressed key.
	i = -1;
	while (keycode_list[++i] != KEY_UNUSED && keycode_list[i] != key);
	if (keycode_list[i] != key)
		return (0);

	// If the key match, update buffer.
	// Get the char to be written
	if ((keyboard->mode & 1) == 0)
		character = (keyboard->mode & 2) ? 'A' + i : 'a' + i;
	else
		character = keylist_num_char[i];
	
	// Insert new character if possible.
	buffer_insert(keyboard, character);
	return (1);
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
	int x;
	int y;

	// Draw cursor if needed
	if (keyboard->cvisible == 0)
	{
		// Geneate TTY buffer cursor position.
		x = keyboard->buffer.cursor + keyboard->saved.tty.cursor.x;
		y = x / keyboard->tty->cursor.max.x;
		x = x - (y * keyboard->tty->cursor.max.x);
		y = y + keyboard->saved.tty.cursor.y;
		
		// Save current cursor position and
		// resotre saved position.
		int sttyx = keyboard->tty->cursor.x;
		int sttyy = keyboard->tty->cursor.x;
		keyboard->tty->cursor.x = x;
		keyboard->tty->cursor.y = y;

		// Get Display X and Y position.
		tty_ioctl(NULL, TTY_IOCTL_GETDX, &x);
		tty_ioctl(NULL, TTY_IOCTL_GETDY, &y);

		// Display cursor.
		kvram_reverse(x, y, (KERNEL_FONT_REAL_WIDTH + 1), (KERNEL_FONT_REAL_HEIGHT + 1));
		kvram_display();

		// Restore TTY cursor position
		keyboard->tty->cursor.x = sttyx;
		keyboard->tty->cursor.y = sttyy;

	}
	// Update cursor status.
	keyboard->cvisible = keyboard->cvisible ^ 1;
}
