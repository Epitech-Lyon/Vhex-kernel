#include <kernel/devices/tty.h>
#include <lib/display.h>

// Internal TTY object.
extern struct tty_s tty;

static void tty_new_line(void)
{
	tty.cursor.x = 0;
	if (tty.cursor.y + 1 >= tty.cursor.max.y)
	{
		dscroll(KERNEL_FONT_REAL_HEIGHT + 1);
		return;
	}
	tty.cursor.y = tty.cursor.y + 1;
	return;
}

static void tty_update_cursor(void)
{
	tty.cursor.x = tty.cursor.x + 1;
	if (tty.cursor.x >= tty.cursor.max.x)
		tty_new_line();
}

int tty_write(char const *str, ...)
{
	int i;

	// Do not handle strig format for now.
	
	// Display string format.
	i = -1;
	while (str[++i] != '\0')
	{
		// Check new line char.
		if (str[i] == '\n')
			tty_new_line();
		// Display char.
		dascii(
			tty.cursor.x * (KERNEL_FONT_REAL_WIDTH + 1),
			tty.cursor.y * (KERNEL_FONT_REAL_HEIGHT + 1),
			str[i]
		);
		tty_update_cursor();
	}
	dupdate();
	return (i);
}
