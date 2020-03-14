#include <kernel/devices/earlyterm.h>
#include <kernel/drivers/screen.h>

// Internal globals
struct earlyterm earlyterm;

// earlyterm_init()
// Initialize internal early terminal symbols
int earlyterm_init(void)
{
	// Get internal vram and font
	// TODO: return menu
	if (dopen(&earlyterm.display, "default") != 0)
		return (-1);

	// Generate cursor informations
	earlyterm.cursor.x = 0;
	earlyterm.cursor.max.x = screen_get(SCREEN_WIDTH);
	earlyterm.cursor.max.x /= earlyterm.display.font->font.width + 1;

	earlyterm.cursor.max.y = screen_get(SCREEN_HEIGHT);
	earlyterm.cursor.max.y /= earlyterm.display.font->font.height + 1;
	earlyterm.cursor.y = 0;
	return (0);
}
