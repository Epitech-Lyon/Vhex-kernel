#ifndef __KERNEL_TTY_H__
# define __KERNEL_TTY_H__

#include <stddef.h>
#include <stdint.h>
#include <kernel/devices/display.h>
#include <kernel/types.h>

// Define default buffer size.
// TODO: remove me ?
#define TTY_BUFFER_LINES	(DISPLAY_SCREEN_WIDTH * 3)
#define TTY_BUFFER_COLUMNS	(DISPLAY_SCREEN_HEIGHT)

struct tty_s
{
	char buffer[TTY_BUFFER_LINES][TTY_BUFFER_COLUMNS];
	struct {
		int16_t x;
		int16_t y;
		struct {
			int16_t x;
			int16_t y;
		} max;
	} cursor;
	struct {
		ssize_t (*write)(const void *buffer, size_t count);
		ssize_t (*read)(void *buffer, size_t count);
	} primitives;
};

// internal strct used by the TTY read primitives
// TODO: Move me ?
struct keyboard_obj_s
{
	struct {
		uint8_t *addr;
		size_t size;
		size_t clen;
		off_t cursor;
	} buffer;
	uint8_t mode;
	uint8_t cvisible;
	struct {
		struct {
			struct {
				int16_t x;
				int16_t y;
			} cursor;
		} tty;
	} saved;
};

// primitives.
extern int tty_open(void);
extern ssize_t tty_write(const void *buffer, size_t count);
extern ssize_t tty_read(void *buffer, size_t count);
extern int tty_close(void);

#endif /*__KERNEL_TTY_H__*/
