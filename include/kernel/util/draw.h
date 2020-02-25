#ifndef __KERNEL_UTIL_DRAW_H__
# define __KERNEL_UTIL_DRAW_H__

#include <stddef.h>
#include <stdint.h>
#include <kernel/drivers/screen.h>

// Define the (hardcoded) font bitmap informations.
#define KERNEL_FONT_BITMAP_WIDTH	(127)	// Bitmap width
#define KERNEL_FONT_BITMAP_HEIGHT	(23)	// Bitmap height
#define KERNEL_FONT_BITMAP_CWIDTH	(4)	// Character width (bitmap)
#define KERNEL_FONT_BITMAP_CHEIGHT	(6)	// Character height (bitmap)
#define KERNEL_FONT_REAL_WIDTH		(3)	// Charater width (real)
#define KERNEL_FONT_REAL_HEIGHT		(5)	// Character height (real)
#define KERNEL_FONT_NB_CHAR_X		((KERNEL_FONT_BITMAP_WIDTH / KERNEL_FONT_BITMAP_CWIDTH) + 1)
#define KERNEL_FONT_NB_CHAR_Y		((KERNEL_FONT_BITMAP_HEIGHT / KERNEL_FONT_BITMAP_CHEIGHT) + 1)

// Define Number of vertical lines
// and horizontal lines.
// TODO: move me to <kernel/drivers/screen.h> ?
#define DISPLAY_VCHAR_MAX		(DISPLAY_SCREEN_HEIGHT / (KERNEL_FONT_REAL_HEIGHT + 1))
#define DISPLAY_HCHAR_MAX		(DISPLAY_SCREEN_WIDTH / (KERNEL_FONT_REAL_WIDTH + 1))



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



// Prototype
extern void kvram_clear(void);
extern void kvram_display(void);
extern void kvram_scroll(int lines);
extern void kvram_reverse(int x, int y, int width, int height);
extern void kvram_clr_str_area(int x, int y, int width, int height);
extern void kvram_print(int x, int y, const char *string, size_t len);
extern void kvram_ascii(int x, int y, char const c);

#endif /*__KERNEL_UTIL_DRAW_H__*/
