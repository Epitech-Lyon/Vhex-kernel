#ifndef __KERNEL_DEVICES_EARLYTERM_H__
# define __KERNEL_DEVICES_EARLYTERM_H__

#include <stddef.h>
#include <stdint.h>
#include <lib/display.h>

// Wait for debug (dirty)
// TODO: use earlyterm_read() to wait key event ?
#define DBG_WAIT for(int i = 0 ; i < 3000000 ; i++)

// Internal earlyterm device struc
struct earlyterm
{
	// Internal object used by the
	// drawing lib
	display_t display;

	// Cursor part, we use only the X
	// axis because we scroll the vram when
	// the line overflow or new line char
	// involved.
	struct {
		uint8_t x;
		uint8_t y;
		struct {
			uint8_t x;
			uint8_t y;
		} max;
	} cursor;
};


// Primitives
extern int earlyterm_init(void);
extern void earlyterm_clear(void);
extern void earlyterm_write(const char *format, ...);

#endif /*__KERNEL_DEVICES_EARLYTERM_H__*/
