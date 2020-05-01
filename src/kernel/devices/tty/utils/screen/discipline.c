#include <kernel/devices/tty.h>

/* tty_vertical_update() - Check / update TTY vertical cursor */
static void tty_vertical_update(struct tty *tty)
{
	tty->screen.cursor.disp.y = tty->screen.cursor.disp.y + 1;
	if (tty->screen.cursor.disp.y >= tty->screen.winsize.ws_row)
	{
		dscroll(&tty->screen.private.disp, tty->screen.private.disp.font->font.height + 1);
		tty->screen.cursor.disp.y = tty->screen.winsize.ws_row - 1;
	}
}

/* tty_horizontal_update() - Check / update TTY horizotal cursor */
static int tty_horizontal_update(struct tty *tty)
{
	tty->screen.cursor.disp.x = tty->screen.cursor.disp.x + 1;
	if (tty->screen.cursor.disp.x >= tty->screen.winsize.ws_col)
	{
		tty->screen.cursor.disp.x = 0;
		tty_vertical_update(tty);
		return (1);
	}
	return (0);
}

/* tty_vertical_clear() - Form feed emulation */
static void tty_vertical_clear(struct tty *tty)
{
	dscroll(
		&tty->screen.private.disp,
		tty->screen.private.disp.font->font.height * tty->screen.winsize.ws_row
	);
}

int tty_screen_discipline_char(struct tty *tty, char n, int action)
{
	int offset;

	switch (n)
	{
		// Check bell char (TODO)
		case '\a': break;

		// Check backspace.
		case '\b':
			   if (tty->screen.cursor.disp.x > 0)
				   tty->screen.cursor.disp.x -= 1;
			   break;

		// Check tabulation
		case '\t':
			// Check if we need a new line or not.
			offset = 5 - (tty->screen.cursor.disp.x - ((tty->screen.cursor.disp.x / 5) * 5));
			if (tty->screen.cursor.disp.x + offset < tty->screen.winsize.ws_col) {
				tty->screen.cursor.disp.x = tty->screen.cursor.disp.x + offset;
				break;
			}

			// If a new line is required char.
			// Generate a new line.
			tty->screen.cursor.disp.x = 0;
			tty_vertical_update(tty);
			break;

		// Check 'form feed'
		// Check 'vertical tab'
		// Check 'new line'
		// Check 'cariage return'
		case '\f': tty->screen.cursor.disp.y = 0; tty_vertical_clear(tty); break;
		case '\v': tty_vertical_update(tty); break;
		case '\n': tty->screen.cursor.disp.x = 0; tty_vertical_update(tty); break;
		case '\r': tty->screen.cursor.disp.x = 0; break;
		default:
			if (action != 0) {
				dascii(&tty->screen.private.disp,
					tty->screen.cursor.disp.x, tty->screen.cursor.disp.y,
					n, 0);
			}
			tty_horizontal_update(tty);
			return (0);
	}
	return (0);
}
