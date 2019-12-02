#include <lib/display.h>

void dscroll(int lines)
{
	int i;

	// Scoll n lines
	i = 0;
	while ((i >> 2) < 63 - lines)
	{
		VRAM[i + 0] = VRAM[i + (lines << 2) + 0];
		VRAM[i + 1] = VRAM[i + (lines << 2) + 1];
		VRAM[i + 2] = VRAM[i + (lines << 2) + 2];
		VRAM[i + 3] = VRAM[i + (lines << 2) + 3];
		i = i + 4;
	}
	// Clear last n lines
	while ((i >> 2) < 64)
	{
		VRAM[i + 0] = 0x0000;
		VRAM[i + 1] = 0x0000;
		VRAM[i + 2] = 0x0000;
		VRAM[i + 3] = 0x0000;
		i = i + 4;
	}
}
