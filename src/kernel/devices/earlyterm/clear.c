#include <kernel/devices/earlyterm.h>
#include <lib/display.h>

/* earlyterm_clear() - clear internal vram and reset cursor position */
void earlyterm_clear(void)
{
	extern struct earlyterm earlyterm;

	// Clear video ram
	dclear(&earlyterm.display);

	// Reset cursor
	earlyterm.cursor.x = 0;
	earlyterm.cursor.y = 0;
}
