#include <kernel/util.h>
#include <kernel/atomic.h>
#include <kernel/devices/display.h>

// Internal Video RAM
uint32_t vram[256];

/* kvram_clear() - Wipe the Video RAM */
void kvram_clear(void)
{
	int i;

	// The Video RAM is shared between each
	// process and the kernel, so we should
	// use atomic operation when we use it.
	atomic_start();

	// Wipe Video RAM
	// @note: here, we suppose that the VRAM is
	// 4-aligned
	i = 256;
	while (--i >= 0)
		vram[i] = 0x00000000;

	// End of atomic operation
	atomic_stop();
}

/* kvram_display() - Disaplay Video RAM into screen */
void kvram_display(void)
{
	//TODO: handle screen hadware !
	t6k11_display(vram);
}

/* kvram_scroll() - Scroll up the Video RAM */
void kvram_scroll(int lines)
{
	int i;

	// The Video RAM is shared between each
	// process and the kernel, so we should
	// use atomic operation when we use it.
	atomic_start();

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

	// End of atomic operation
	atomic_stop();
}

/* kvram_reverse() - Reverse Video RAM area */
void kvram_reverse(int x, int y, int width, int height)
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

/* kvram_clr_str_area() - Clear area based on kernel font size on Video RAM */
//FIXME: secure
void kvram_clr_str_area(int x, int y, int width, int height)
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

	//DEbug
	/*kvram_clear();
	printk(0, 0, "kvram_clr_str_area debug !");
	printk(0, 1, "x = %d", x);
	printk(0, 2, "y = %d", y);
	printk(0, 3, "width = %d", width);
	printk(0, 4, "height = %d", height);
	kvram_display();
	DBG_WAIT;*/

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

}

/* kvram_ascii() - Draw ASCII character into Video RAM */
void kvram_ascii(int x, int y, char const c)
{
	font_draw(x, y, c);
}
