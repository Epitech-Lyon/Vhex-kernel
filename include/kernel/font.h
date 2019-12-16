#ifndef __KERNEL_FONT_H__
# define __KERNEL_FONT_H__

#include <stddef.h>
#include <stdint.h>

// Define font bitmap informations.
#define KERNEL_FONT_BITMAP_WIDTH	(127)	// Bitmap width
#define KERNEL_FONT_BITMAP_HEIGHT	(23)	// Bitmap height
#define KERNEL_FONT_BITMAP_CWIDTH	(4)	// Character width (bitmap)
#define KERNEL_FONT_BITMAP_CHEIGHT	(6)	// Character height (bitmap)
#define KERNEL_FONT_REAL_WIDTH		(3)	// Charater width (real)
#define KERNEL_FONT_REAL_HEIGHT		(5)	// Character height (real)
#define KERNEL_FONT_NB_CHAR_X		((KERNEL_FONT_BITMAP_WIDTH / KERNEL_FONT_BITMAP_CWIDTH) + 1)
#define KERNEL_FONT_NB_CHAR_Y		((KERNEL_FONT_BITMAP_HEIGHT / KERNEL_FONT_BITMAP_CHEIGHT) + 1)

// Internal struct used to draw
// the ASCII character.
struct font_block_s
{
	int16_t height;
	int16_t width;
	struct {
		uint16_t x;
		uint16_t y;
	} bitmap;
	int16_t x;
	int16_t y;
};

// Font function.
extern void font_draw(int x, int y, char n);

#endif /*__KERNEL_FONT_H__*/
