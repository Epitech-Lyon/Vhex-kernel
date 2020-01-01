#include <kernel/util.h>
#include <kernel/font.h>
#include <stdarg.h>

void printk(int x, int y, char const *str, ...)
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

	// End of variodic args.
	va_end(ap);
}
