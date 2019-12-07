#include <lib/stdio.h>
#include <lib/display.h>
#include <stdarg.h>

void dprint(int x, int y, char const *str, ...)
{
	char buffer[512];
	int default_pos_x;
	va_list ap;
	int i;

	// printf string manipulation.
	// TODO: remove me !
	va_start(ap, str);
	vsprintf(buffer, str, ap);
	va_end(ap);

	i = -1;
	x = x * (KERNEL_FONT_REAL_WIDTH + 1);
	y = y * (KERNEL_FONT_REAL_HEIGHT + 1);
	default_pos_x = x;
	while (buffer[++i] != '\0')
	{
		if (buffer[i] == '\n')
		{
			y = y + KERNEL_FONT_REAL_HEIGHT + 1;
			x = default_pos_x;
			continue;
		}
		dascii(x, y, buffer[i]);
		x = x + KERNEL_FONT_REAL_WIDTH + 1;
	}
}
