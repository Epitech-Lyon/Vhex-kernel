#ifndef __KERNEL_TTY_H__
# define __KERNEL_TTY_H__

#include <stddef.h>
#include <stdint.h>
#include <sys/types.h>
#include <kernel/drivers/keyboard.h>
#include <display.h>
#include <pthread.h>

// Define TTY major
#define TTY_DEV_MAJOR		(4)

// Internal TTY informations
#define TTY_INPUT_BUFFSIZE	(256)
#define TTY_OUTPUT_BUFFSIZE	(1024)

struct tty
{
	// Internal buffers
	// TODO: move me ?
	struct {
		char input[TTY_INPUT_BUFFSIZE];
		char output[TTY_OUTPUT_BUFFSIZE];
	} buffers;

	// Screen abstraction part
	struct {
		// Window size informations used by the write primitive
		struct {
			unsigned short ws_col;
			unsigned short ws_row;
			unsigned short ws_xpixel;
			unsigned short ws_ypixel;
		} winsize;

		// Cursor informations
		struct {
			// Drawing cursors
			struct {
				int16_t x;
				int16_t y;
			} disp;

			// Internal output buffer cursor
			int write;
			int flush;
		} cursor;

		// Private data
		struct {
			// Object used by the drawing library.
			// It's store video ram and font informations
			display_t disp;

			// The timer is used to flush the internal
			// output buffer
			struct {
				// Timer ID used to flush the output
				// buffer on the screen
				int id;

				// Used to indicate that the output flush timer
				// is currently used
				int status;
			} timer;

			// used to indicate if we want to not use the cache
			int nocache;

			// screen abstraction mutex
			pthread_mutex_t mutex;
		} private;
	} screen;


	// Internal keyboard informations
	struct {
		// Internal keyboard mode for cannonical (cooked)
		// keyboard configurations
		struct {
			uint8_t enter	: 1;
			uint8_t maj	: 1;
			uint8_t special	: 1;
			uint8_t ctrl	: 1;
			uint8_t const	: 4;
		} mode;

		// Internal saved values, used for the echoing
		// mecanism
		struct {
			struct {
				struct {
					int16_t x;
					int16_t y;
				} cursor;
			} tty;
			struct {
				uint8_t keycode;
				off_t cursor;
			} signal;
		} saved;

		// Private data
		// TODO: keyboard configuration ? (raw / cannonical)
		struct {
			// Internal cursor informations for the input
			// buffer
			off_t cursor;
			off_t cursor_max;

			// The timer is used to blink the currsor position
			struct {
				// Timer ID
				int id;

				// Used to count the number of "timer_start()"
				// and known if we can stop the timer.
				int used;
			} timer;

			// Internal mutex to avoid atomic operation
			pthread_mutex_t mutex;
		} private;
	} keyboard;

	// Internal device data
	struct {
		int open_cnt;
	} private;
};

// primitives.
extern void *tty_open(dev_t major, dev_t minor);
extern ssize_t tty_write(void *inode, const void *buffer, size_t count);
extern ssize_t tty_read(void *inode, void *buffer, size_t count);
extern int tty_close(void *inode);

// Internal utilities (keyboard abstraction)
extern int tty_keyboard_constructor(struct tty *tty);
extern int tty_keyboard_buffer_insert(struct tty *tty, char n);
extern int tty_keyboard_buffer_update(struct tty *tty, key_t key);
extern int tty_keyboard_check_special(struct tty *tty, key_t key);
extern void tty_keyboard_check_signal(struct tty *tty);
extern void tty_keyboard_buffer_remove(struct tty *tty, off_t cursor);
extern void tty_keyboard_buffer_flush(struct tty *tty);
extern void tty_keyboard_buffer_echoing(struct tty *tty);
extern void tty_keyboard_destructor(struct tty *tty);

// Internal utilities (screen abstraction)
extern int tty_screen_constructor(struct tty *tty);
extern ssize_t tty_screen_buffer_update(struct tty *tty, const uint8_t *buffer, size_t count);
extern void tty_screen_buffer_flush(struct tty *tty);
extern void tty_screen_display_callback(struct tty *tty);
extern int tty_screen_discipline_char(struct tty *tty, char n, int action);
extern void tty_screen_destructor(struct tty *tty);

// IOCTL (remove me ?)
#define TTY_IOCTL_GETDX	(0x00)
#define TTY_IOCTL_GETDY	(0x01)
extern void tty_ioctl(void *inode, uint32_t cmd, ...);

#endif /*__KERNEL_TTY_H__*/
