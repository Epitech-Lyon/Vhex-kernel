#include <kernel/devices/tty.h>
#include <kernel/devices/earlyterm.h>
#include <kernel/drivers/screen.h>
#include <kernel/memory.h>
#include <lib/display.h>
#include <lib/string.h>

void *tty_open(dev_t major, dev_t minor)
{
	struct tty_s *tty;
	int line;

	// TODO: handle major / minor !!
	(void)major;
	(void)minor;
	
	// Try to allocate tty object
	tty = (struct tty_s*)pm_alloc(sizeof(struct tty_s));
	if (tty == NULL)
		return (NULL);

	// Get the font used by the tty.
	if (dopen(&tty->disp, "default") != 0)
	{
		pm_free(tty);
		return (NULL);
	}

	// Generate "visible" informations for the write primitive
	tty->winsize.ws_xpixel = screen_get(SCREEN_WIDTH);
	tty->winsize.ws_ypixel = screen_get(SCREEN_HEIGHT);
	tty->winsize.ws_col = tty->winsize.ws_xpixel / (tty->disp.font->font.width + 1);
	tty->winsize.ws_row = tty->winsize.ws_ypixel / (tty->disp.font->font.height + 1);
	
	// Generate formated output buffer indicator
	tty->cursor.max.x = screen_get(SCREEN_WIDTH) / (tty->disp.font->font.width + 1);
	tty->cursor.max.y = screen_get(SCREEN_HEIGHT) / (tty->disp.font->font.height + 1);
	tty->cursor.max.y = tty->cursor.max.y * 4;

	// Try to alloc new tty output buffer
	tty->buffers.output = (char **)pm_alloc(sizeof(char *) * tty->cursor.max.y);
	if (tty->buffers.output == NULL)
	{
		pm_free(tty);
		return (NULL);
	}

	// Try to alloc each line of the output buffer
	line = tty->cursor.max.y;
	while (--line >= 0)
	{
		// Try to alloc the line
		tty->buffers.output[line] = (char*)pm_alloc(tty->cursor.max.x);
		if (tty->buffers.output[line] != NULL) {
			memset(tty->buffers.output[line], '\0', tty->cursor.max.x);
			continue;
		}
		// Release all allocated space and return NULL
		while (++line < tty->cursor.max.y)
			pm_free(tty->buffers.output[line]);
		pm_free(tty->buffers.output);
		pm_free(tty);
		return (NULL);
	}

	// Initialize default TTY cursor position.
	tty->cursor.x = 0;
	tty->cursor.y = 0;
	return (tty);
}
