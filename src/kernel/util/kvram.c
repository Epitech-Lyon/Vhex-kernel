#include <kernel/util.h>
#include <kernel/atomic.h>
#include <kernel/devices/display.h>
#include <stdarg.h>

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
	atomic_end();
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
	atomic_end();
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
	atomic_end();
}

/* kvram_print() - Print formated string on Video RAM */
void kvram_print(int x, int y, char const *str, ...)
{
	char hex[] = "0123456789abcdef";
	char buffer[16];
	int default_pos_x;
	int starting_x;
	int digits;
	int32_t nb;
	uint8_t base;
	va_list ap;
	int i;

	// Get starting variable args
	va_start(ap, str);

	// Initialize part
	i = -1;
	starting_x = x;
	x = x * (KERNEL_FONT_REAL_WIDTH + 1);
	y = y * (KERNEL_FONT_REAL_HEIGHT + 1);
	default_pos_x = x;

	// Walk into string and display character by character
	while (str[++i] != '\0')
	{
		// New line
		if (str[i] == '\n')
		{
			y = y + KERNEL_FONT_REAL_HEIGHT + 1;
			x = default_pos_x;
			continue;
		}
		// Horizontal tab
		if (str[i] == '\t')
		{
			x = x / (KERNEL_FONT_REAL_WIDTH + 1);
			x = (x + (4 - ((x - starting_x) & 3))) * (KERNEL_FONT_REAL_WIDTH + 1);
			continue;
		}
		// String format "simple"
		if (str[i] == '%')
		{
			if (str[i + 1] == 'd' || str[i + 1] == 'x')
			{
				// Initialise
				digits = 0;
				nb = va_arg(ap, int32_t);
				base = (str[i + 1] == 'd') ? 10 : 16;

				// Generate buffer
				// @note: generate in LSB mode.
				while (digits == 0 || nb != 0)
				{
					buffer[digits++] = hex[nb % base];
					nb = nb / base;
				}

				// Reverse dans display string
				while (--digits >= 0)
				{
					font_draw(x, y, buffer[digits]);
					x = x + KERNEL_FONT_REAL_WIDTH + 1;
				}
				i = i + 1;
				continue;
			}
			if ((str[i + 1] == '#' && str[i + 2] == 'x') || str[i + 1] == 'p')
			{
				// add @ if 'p' (pointer)
				if (str[i + 1] == 'p')
				{
					font_draw(x, y, '@');
					x = x + KERNEL_FONT_REAL_WIDTH + 1;
				}

				// Add "0x"
				font_draw(x, y, '0');
				x = x + KERNEL_FONT_REAL_WIDTH + 1;
				font_draw(x, y, 'x');
				x = x + KERNEL_FONT_REAL_WIDTH + 1;

				// Get value
				digits = 0;
				nb = va_arg(ap, uint32_t);
				while (digits < 8)
				{
					buffer[digits++] = hex[nb & 0x0f];
					nb = nb >> 4;
				}

				// Display string.
				while (--digits >= 0)
				{
					font_draw(x, y, buffer[digits]);
					x = x + KERNEL_FONT_REAL_WIDTH + 1;
				}
				
				// Update cursor
				i = (str[i + 1] == '#') ? i + 2 : i + 1;
				continue;
			}
			if (str[i + 1] == 's')
			{
				digits = 0;
				nb = va_arg(ap, uint32_t);
				while (((char*)nb)[digits] != '\0')
				{
					font_draw(x, y, ((char*)nb)[digits++]);
					x = x + KERNEL_FONT_REAL_WIDTH + 1;
				}
				i = i + 1;
				continue;
			}
		}

		// Default, display character
		font_draw(x, y, str[i]);
		x = x + KERNEL_FONT_REAL_WIDTH + 1;
	}

	va_end(ap);
}

/* kvram_ascii() - Draw ASCII character into Video RAM */
void kvram_ascii(int x, int y, char const c)
{
	font_draw(x, y, c);
}
