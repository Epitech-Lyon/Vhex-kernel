//#include <kernel/util/draw.h>
//#include <kernel/devices/display.h>
//#include <kernel/atomic.h>

/* kvram_clr_str_area() - Clear area based on kernel font size on Video RAM */
//FIXME: secure (DO NOT USE IT !!!!!!)
/*void kvram_clr_str_area(int x, int y, int width, int height)
{
	uint32_t vram_offset_y;
	int cur_x;
	int cur_y;

	// Initialize part
	x = x * (KERNEL_FONT_REAL_WIDTH + 1);
	y = y * (KERNEL_FONT_REAL_HEIGHT + 1);
	width = width * (KERNEL_FONT_REAL_WIDTH + 1);
	height = height * (KERNEL_FONT_REAL_HEIGHT + 1);

	// Get VRAM offset
	vram_offset_y = y << 2;

	// Start atomic operations
	atomic_start();

	// Clear area, pixel per pixel x____x
	// TODO: update me !!!!
	cur_y = -1;
	while (++cur_y < height)
	{
		cur_x = -1;
		while (++cur_x < width)
		{
			vram[((x + cur_x) >> 5) + vram_offset_y] &=
				~(0x80000000 >> ((x + cur_x) & 31));
		}

		// update internal counter
		vram_offset_y = vram_offset_y + 4;
	}

	// Stop atomic operations
	atomic_stop();
}*/
