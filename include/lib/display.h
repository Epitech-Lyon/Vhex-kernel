#ifndef __LIB_DISPLAY_H__
# define __LIB_DISPLAY_H__

#include <stddef.h>
#include <stdint.h>


// Define font bitmap informations.
#define KERNEL_FONT_BITMAP_WIDTH	127	// Bitmap width
#define KERNEL_FONT_BITMAP_HEIGHT	23	// Bitmap height
#define KERNEL_FONT_BITMAP_CWIDTH	4	// Character width (bitmap)
#define KERNEL_FONT_BITMAP_CHEIGHT	6	// Character height (bitmap)
#define KERNEL_FONT_REAL_WIDTH		3	// Charater width (real)
#define KERNEL_FONT_REAL_HEIGHT		5	// Character height (real)
#define KERNEL_FONT_NB_CHAR_X		((KERNEL_FONT_BITMAP_WIDTH / KERNEL_FONT_BITMAP_CWIDTH) + 1)
#define KERNEL_FONT_NB_CHAR_Y		((KERNEL_FONT_BITMAP_HEIGHT / KERNEL_FONT_BITMAP_CHEIGHT) + 1)

// Define screen informations.
#define DISPLAY_SCREEN_WIDTH	128
#define DISPLAY_SCREEN_HEIGHT	64

// Primtives
extern void dclear(void);
extern void dprint(int x, int y, char const *str, ...);
extern void dascii(int x, int y, char const c);
extern void dscroll(int line);
extern void dupdate(void);

// Sheared Video RAM
extern uint8_t *vram;

#endif /*__LIB_DISPLAY_H__*/
