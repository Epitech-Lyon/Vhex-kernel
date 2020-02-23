#include <kernel/util/draw.h>
#include <kernel/devices/display.h>
#include <kernel/util/atomic.h>

/* kvram_reverse() - Reverse Video RAM area */
void kvram_reverse(int x, int y, int width, int height)
{
	extern uint32_t vram[256];
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
	// @note:
	// 	The screen width size is always 128 and we
	// use 4-aligned Video RAM so 32 pixels per "slot"
	// and 128 / 32 = 4.
	// y * 4 can be optimised by used shift operator,
	// this is why we use y << 2 because 2^2 = 4.
	vram_offset_y = (y + height - 1) << 2;


	// The Video RAM is sheared between each
	// process and the kernel, so we should
	// use atomic operation when we use it.
	atomic_start();

	// Reverse area
	while (--height >= 0)
	{
		j = width + x;
		while (--j >= x)
		{
			vram[(j >> 5) + vram_offset_y] ^= 0x80000000 >> (j & 31);
		}
		vram_offset_y = vram_offset_y - 4;
	}

	// End of atomic operation
	atomic_stop();
}
