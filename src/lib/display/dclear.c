#include <lib/display.h>

/* dclear() - Wipe the Video RAM */
void dclear(display_t *disp)
{
	int i;

	// Wipe Video RAM
	// @note: here, we suppose that the VRAM
	// is 4-aligned.
	i = 256;
	while (--i >= 0)
		disp->vram[i] = 0x00000000;

}
