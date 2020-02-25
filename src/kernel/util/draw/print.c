#include <kernel/util/draw.h>

void kvram_print(int x, int y, char const *string, size_t len)
{
	size_t i;

	// Generate pixel positions
	x = x * (KERNEL_FONT_REAL_WIDTH + 1);
	y = y * (KERNEL_FONT_REAL_HEIGHT + 1);

	i = -1;
	while (++i < len)
	{
		// display ASCII char
		kvram_ascii(x, y, string[i]);

		// Update X position.
		x = x + KERNEL_FONT_REAL_WIDTH + 1;
	}
}
