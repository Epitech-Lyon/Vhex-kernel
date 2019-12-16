#include <lib/display.h>
#include <kernel/devices/display.h>

void dscroll(int lines)
{
	int i;

	// Scoll n lines
	i = 0;
	while ((i >> 2) < 63 - lines)
	{
		vram[i + 0] = vram[i + (lines << 2) + 0];
		vram[i + 1] = vram[i + (lines << 2) + 1];
		vram[i + 2] = vram[i + (lines << 2) + 2];
		vram[i + 3] = vram[i + (lines << 2) + 3];
		i = i + 4;
	}
	// Clear last n lines
	while ((i >> 2) < 64)
	{
		vram[i + 0] = 0x00000000;
		vram[i + 1] = 0x00000000;
		vram[i + 2] = 0x00000000;
		vram[i + 3] = 0x00000000;
		i = i + 4;
	}
}
