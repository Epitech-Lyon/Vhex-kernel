#include <lib/display.h>

/* dscroll() - Scroll up the Video RAM */
//FIXME: This part is hardware specific (128x64px)!!
void dscroll(display_t *disp, int lines)
{
	int i;

	// Scoll n lines
	i = 0;
	while ((i >> 2) < 63 - lines)
	{
		disp->vram[i + 0] = disp->vram[i + (lines << 2) + 0];
		disp->vram[i + 1] = disp->vram[i + (lines << 2) + 1];
		disp->vram[i + 2] = disp->vram[i + (lines << 2) + 2];
		disp->vram[i + 3] = disp->vram[i + (lines << 2) + 3];
		i = i + 4;
	}
	// Clear last n lines
	while ((i >> 2) < 64)
	{
		disp->vram[i + 0] = 0x00000000;
		disp->vram[i + 1] = 0x00000000;
		disp->vram[i + 2] = 0x00000000;
		disp->vram[i + 3] = 0x00000000;
		i = i + 4;
	}
}
