#include <kernel/devices/tty.h>
#include <lib/display.h>

// Internal TTY object.
struct tty_s tty;

int tty_open(void)
{
	// Initialize TTY.
	tty.cursor.x = 0;
	tty.cursor.y = 0;
	tty.cursor.max.x = DISPLAY_SCREEN_WIDTH / (KERNEL_FONT_REAL_WIDTH + 1);
	tty.cursor.max.y = DISPLAY_SCREEN_HEIGHT / (KERNEL_FONT_REAL_HEIGHT + 1);

	// Clear VRAM.
	dclear();
	return (0);
}
