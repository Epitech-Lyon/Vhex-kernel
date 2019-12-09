#include <lib/display.h>

void dreverse(int x, int y, int width, int height)
{
	int vram_offset_y;
	int j;

	// Check error.
	if (width < 0 || height < 0)
		return;

	// Get "real" X position and area width.
	if (x < 0)
	{
		width = width + x;
		x = 0;
	} else {
		if (x + width >= DISPLAY_SCREEN_WIDTH)
			width = DISPLAY_SCREEN_WIDTH - x;
	}

	// Get "real" Y position and area height.
	if (y < 0)
	{
		height = height + x;
		y = 0;
	} else {
		if (y + height >= DISPLAY_SCREEN_HEIGHT)
			height = DISPLAY_SCREEN_HEIGHT - x;
	}

	// Check potential error.
	// @note we do not check height because the while()
	// while do the job for us.
	if (width < 0)
		return;

	// Generate VRAM offset for Y axis.
	vram_offset_y = (y + height - 1) << 4;

	while (--height >= 0)
	{
		j = width + x;
		while (--j >= x)
		{
			vram[(j >> 3) + vram_offset_y] ^= 0x80 >> (j & 7);
		}
		vram_offset_y = vram_offset_y - 16;
	}
}
