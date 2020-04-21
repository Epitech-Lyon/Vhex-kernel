#ifndef __KERNEL_TTY_H__
# define __KERNEL_TTY_H__

#include <stddef.h>
#include <stdint.h>
#include <sys/types.h>
#include <kernel/drivers/screen.h>
#include <display.h>

// Define default buffer size.
// TODO: remove me ?
//#define TTY_BUFFER_LINES	(DISPLAY_VCHAR_MAX * 3)
//#define TTY_BUFFER_COLUMNS	(DISPLAY_HCHAR_MAX)

// Define TTY major
#define TTY_DEV_MAJOR		(4)

struct tty_s
{
	// Internal buffers
	struct {
		char input[256];
		char **output;
	} buffers;

	// Window size informations used by the write primitive
	struct {
		unsigned short ws_col;
		unsigned short ws_row;
		unsigned short ws_xpixel;
		unsigned short ws_ypixel;
	} winsize;

	// Cursor informations
	struct {
		int16_t x;
		int16_t y;
		struct {
			int16_t x;
			int16_t y;
		} max;
	} cursor;

	// Object used by the drawing library.
	// It's store video ram and font informations
	display_t disp;
};

// internal strct used by the TTY read primitives
// TODO: Move me ?
// TODO: Add internal keyboard buffer  !!!
struct keyboard_obj_s
{
	struct {
		uint8_t *addr;
		size_t size;
		size_t clen;
		off_t cursor;
	} buffer;
	struct {
		uint8_t enter	: 1;
		uint8_t maj	: 1;
		uint8_t special	: 1;
		uint8_t ctrl	: 1;
		uint8_t const	: 4;
	} mode;
	uint8_t cvisible;
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

	// FIXME Dirty place, remove / move me !
	struct tty_s *tty;
};

// primitives.
extern void *tty_open(dev_t major, dev_t minor);
extern ssize_t tty_write(void *inode, const void *buffer, size_t count);
extern ssize_t tty_read(void *inode, void *buffer, size_t count);
extern int tty_close(void *inode);

// IOCTL.
#define TTY_IOCTL_GETDX	(0x00)
#define TTY_IOCTL_GETDY	(0x01)
extern void tty_ioctl(void *inode, uint32_t cmd, ...);

#endif /*__KERNEL_TTY_H__*/
