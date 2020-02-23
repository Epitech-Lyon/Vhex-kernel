#ifndef __KERNEL_TTY_H__
# define __KERNEL_TTY_H__

#include <stddef.h>
#include <stdint.h>
#include <kernel/devices/display.h>
#include <kernel/devices/device.h>
#include <kernel/util/types.h>

// Define default buffer size.
// TODO: remove me ?
#define TTY_BUFFER_LINES	(DISPLAY_VCHAR_MAX * 3)
#define TTY_BUFFER_COLUMNS	(DISPLAY_HCHAR_MAX)

// Define TTY major
#define TTY_DEV_MAJOR		(4)

struct tty_s
{
	char buffer[TTY_BUFFER_LINES + 1][TTY_BUFFER_COLUMNS + 1];
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
extern void *tty_open(dev_t major, dev_t minor);
extern ssize_t tty_write(void *inode, const void *buffer, size_t count);
extern ssize_t tty_read(void *inode, void *buffer, size_t count);
extern int tty_close(void *inode);

// IOCTL.
#define TTY_IOCTL_GETDX	(0x00)
#define TTY_IOCTL_GETDY	(0x01)
extern void tty_ioctl(void *inode, uint32_t cmd, ...);

#endif /*__KERNEL_TTY_H__*/
