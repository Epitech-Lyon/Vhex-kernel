#ifndef __KERNEL_TTY_H__
# define __KERNEL_TTY_H__

#include <stddef.h>
#include <stdint.h>

struct tty_s
{
	struct {
		uint8_t x;
		uint8_t y;
		struct {
			uint8_t x;
			uint8_t y;
		} max;
	} cursor;
};

// primitives.
extern int tty_open(void);
extern int tty_write(char const *buffer, ...);
//extern void tty_read(char *buffer, size_t count);
extern int tty_close(void);

#endif /*__KERNEL_TTY_H__*/
