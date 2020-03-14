#include <kernel/drivers/screen.h>

// Internal drivers
extern void t6k11_variant_lcd_driver(void *vram);
extern void t6k11_lcd_driver(void *vram);

// Primitives
void (*screen_update)(void *vram);

__attribute__((constructor))
static void screen_constructor(void)
{
	// Check T6K11 variant which appear during
	// the OS >= 03.00.0000
	if (*(uint8_t*)0xa0010021 == '3') {
		screen_update = (void*)&t6k11_variant_lcd_driver;
		return;
	}
	// Default driver
	screen_update = &t6k11_lcd_driver;
}
