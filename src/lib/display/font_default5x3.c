#include <lib/display.h>

// Define the (hardcoded) font bitmap informations.
#define DEFAULT_FONT_BITMAP_WIDTH	(127)	// Bitmap width
#define DEFAULT_FONT_BITMAP_HEIGHT	(23)	// Bitmap height
#define DEFAULT_FONT_BITMAP_CWIDTH	(4)	// Character width (bitmap)
#define DEFAULT_FONT_BITMAP_CHEIGHT	(6)	// Character height (bitmap)
#define DEFAULT_FONT_REAL_WIDTH		(3)	// Charater width (real)
#define DEFAULT_FONT_REAL_HEIGHT	(5)	// Character height (real)

// Font bitmap (raw).
static const uint8_t default_font_bitmap[] = {
	0xee, 0xee, 0xee, 0xee, 0xee, 0xee, 0xee, 0xee, 0xee, 0xee, 0xee, 0xee,
	0xee, 0xee, 0xee, 0xef, 0x55, 0x55, 0x55, 0x55, 0x55, 0x55, 0x55, 0x55,
	0x55, 0x55, 0x55, 0x55, 0x55, 0x55, 0x55, 0x56, 0xaa, 0xaa, 0xaa, 0xaa,
	0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xad,
	0x55, 0x55, 0x55, 0x55, 0x55, 0x55, 0x55, 0x55, 0x55, 0x55, 0x55, 0x55,
	0x55, 0x55, 0x55, 0x5e, 0xee, 0xee, 0xee, 0xee, 0xee, 0xee, 0xee, 0xee,
	0xee, 0xee, 0xee, 0xee, 0xee, 0xee, 0xee, 0xe0, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01,
	0x2a, 0x92, 0x11, 0x11, 0x28, 0x00, 0x00, 0x38, 0xbb, 0xab, 0xbb, 0xbb,
	0x80, 0x08, 0x23, 0x02, 0x57, 0x31, 0x52, 0x41, 0x22, 0x00, 0x01, 0x51,
	0x11, 0x54, 0x41, 0x55, 0x22, 0x27, 0x21, 0x04, 0x0a, 0xc4, 0x40, 0x82,
	0xae, 0x0e, 0x04, 0xa2, 0xee, 0xee, 0xe2, 0xee, 0x00, 0x80, 0x24, 0x00,
	0x1c, 0xd1, 0x41, 0x04, 0x08, 0x80, 0x11, 0x45, 0x04, 0x45, 0x45, 0x44,
	0x88, 0x9c, 0x80, 0x10, 0x2b, 0x09, 0x81, 0x10, 0x02, 0x01, 0x03, 0x8b,
	0xb8, 0xbb, 0x8b, 0xb8, 0x20, 0x82, 0x10, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0c, 0xee,
	0xec, 0xee, 0xea, 0xee, 0xa8, 0xae, 0xee, 0xee, 0x6e, 0xaa, 0xaa, 0xae,
	0xe0, 0xe4, 0x05, 0x55, 0x15, 0x11, 0x14, 0x89, 0x51, 0xd5, 0x55, 0x55,
	0x09, 0x55, 0x55, 0x45, 0x10, 0x54, 0x0b, 0xb2, 0x2b, 0xba, 0xb9, 0x13,
	0x22, 0xaa, 0xba, 0xb3, 0x92, 0xaa, 0x91, 0x12, 0x10, 0x80, 0x55, 0x54,
	0x54, 0x45, 0x52, 0x25, 0x45, 0x55, 0x47, 0x51, 0x25, 0x57, 0x52, 0x44,
	0x11, 0x00, 0xca, 0xee, 0xce, 0x8e, 0xae, 0xca, 0xea, 0xae, 0x8e, 0xac,
	0x4e, 0x4a, 0xa4, 0xee, 0x0e, 0x0e, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x12, 0x00,
	0x90, 0x9a, 0x11, 0x23, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x19,
	0x30, 0x39, 0x14, 0x31, 0x52, 0x54, 0x00, 0x42, 0x76, 0x26, 0x35, 0x37,
	0x55, 0x55, 0x57, 0x22, 0x20, 0x70, 0x6c, 0x86, 0xee, 0x6c, 0xc4, 0xa4,
	0xea, 0xaa, 0xac, 0xc4, 0xaa, 0xa4, 0xa4, 0x84, 0x26, 0xe1, 0x55, 0x15,
	0x08, 0x54, 0x89, 0x89, 0x55, 0x58, 0xd0, 0x49, 0x55, 0xd4, 0x90, 0x88,
	0x99, 0xc1, 0xb1, 0x99, 0x93, 0x2b, 0xb2, 0x9a, 0xa9, 0x20, 0xa3, 0x19,
	0x92, 0xa9, 0x39, 0x93, 0x03, 0x80
};

// Internal global font object
struct font_s default5x3 = {
	.bitmap = {
		.width = DEFAULT_FONT_BITMAP_WIDTH,
		.height = DEFAULT_FONT_BITMAP_HEIGHT,
		.cwidth = DEFAULT_FONT_BITMAP_CWIDTH,
		.cheight = DEFAULT_FONT_BITMAP_CHEIGHT,
		.raw = (void*)default_font_bitmap
	},
	.font = {
		.width = DEFAULT_FONT_REAL_WIDTH,
		.height = DEFAULT_FONT_REAL_HEIGHT
	}
};