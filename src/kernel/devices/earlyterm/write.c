#include <kernel/devices/earlyterm.h>
#include <kernel/drivers/screen.h>
#include <kernel/util/atomic.h>
#include <stdarg.h>

/* earlyterm_vertical_update() - Check / update device vertical cursor */
static void earlyterm_vertical_update(struct earlyterm *earlyterm)
{
	// Check if we can move the cursor
	if (earlyterm->cursor.y + 1 < earlyterm->cursor.max.y) {
		earlyterm->cursor.y = earlyterm->cursor.y + 1;
		return;
	}

	// Other wise scroll the vram
	dscroll(&earlyterm->display, earlyterm->display.font->font.height + 1);
}

/* earlyterm_horizontal_update() - Check / update earlyterm horizotal cursor */
static int earlyterm_horizontal_update(struct earlyterm *earlyterm)
{
	earlyterm->cursor.x = earlyterm->cursor.x + 1;
	if (earlyterm->cursor.x >= earlyterm->cursor.max.x)
	{
		earlyterm_vertical_update(earlyterm);
		earlyterm->cursor.x = 0;
		return (1);
	}
	return (0);
}

/* print_base() - display number in base (0~16)*/
static void print_base(struct earlyterm *earlyterm, uint32_t nb,
		int base, int digits_min)
{
	char hex[] = "0123456789abcdef";
	char buffer[16];
	int digits;

	// Generate string (reverse)
	digits = 0;
	while (digits < digits_min || nb != 0)
	{
		buffer[digits++] = hex[nb % base];
		nb = nb / base;
	}

	// Display string.
	while (--digits >= 0)
	{
		dascii(&earlyterm->display, earlyterm->cursor.x,
				earlyterm->cursor.y, buffer[digits]);
		earlyterm_horizontal_update(earlyterm);
	}
}

/* line_discipline() - Check all "special" char */
static int line_discipline(struct earlyterm *earlyterm, char n)
{
	// New line
	if (n == '\n')
	{
		earlyterm->cursor.x = 0;
		earlyterm_vertical_update(earlyterm);
		return (1);
	}
	return (0);
}

// earlyterm_write() - printf wrapper for early terminal device
// TODO: update me... x_x
void earlyterm_write(const char *format, ...)
{
	extern struct earlyterm earlyterm;
	int32_t nb;
	va_list ap;
	int i;

	// Atomic start
	atomic_start();

	// Get starting variable args
	va_start(ap, format);

	// Walk into string and display character by character
	i = -1;
	while (format[++i] != '\0')
	{
		// Check line discipline
		if (line_discipline(&earlyterm, format[i]) != 0)
			continue;

		// String format "simple"
		if (format[i] == '%')
		{
			if (format[i + 1] == 'd' || format[i + 1] == 'x')
			{
				// Check negative value
				// FIXME: negative error (max)
				nb = va_arg(ap, int32_t);
				if (nb < 0 && format[i + 1] == 'd')
				{
					nb = 0 - nb;
					dascii(&earlyterm.display, earlyterm.cursor.x, earlyterm.cursor.y, '-');
					earlyterm_horizontal_update(&earlyterm);
				}
				print_base(&earlyterm, nb, (format[i + 1] == 'd') ? 10 : 16, 1);
				i = i + 1;
				continue;
			}
			if ((format[i + 1] == '#' && format[i + 2] == 'x') || format[i + 1] == 'p')
			{
				// add @ if 'p' (pointer)
				if (format[i + 1] == 'p')
				{
					dascii(&earlyterm.display, earlyterm.cursor.x, earlyterm.cursor.y, '@');
					earlyterm_horizontal_update(&earlyterm);
				}

				// Add "0x"
				dascii(&earlyterm.display, earlyterm.cursor.x, earlyterm.cursor.y, '0');
				earlyterm_horizontal_update(&earlyterm);
				dascii(&earlyterm.display, earlyterm.cursor.x, earlyterm.cursor.y, 'x');
				earlyterm_horizontal_update(&earlyterm);

				// Get value
				nb = va_arg(ap, uint32_t);
				print_base(&earlyterm, nb, 16, 8);
				
				// Update cursor
				i = (format[i + 1] == '#') ? i + 2 : i + 1;
				continue;
			}
			if (format[i + 1] == 's')
			{
				nb = va_arg(ap, uint32_t) - 1;
				while (*(char*)(++nb) != '\0')
				{
					if (line_discipline(&earlyterm, *(char*)nb) != 0)
						continue;
					dascii(&earlyterm.display, earlyterm.cursor.x, earlyterm.cursor.y, *(char*)nb);
					earlyterm_horizontal_update(&earlyterm);
				}
				i = i + 1;
				continue;
			}
		}

		// Default, display character
		dascii(&earlyterm.display, earlyterm.cursor.x, earlyterm.cursor.y, format[i]);
		earlyterm_horizontal_update(&earlyterm);
	}

	// Display vram
	screen_update(earlyterm.display.vram);

	// End of variodic args.
	va_end(ap);

	// Atomic stop
	atomic_stop();
}
