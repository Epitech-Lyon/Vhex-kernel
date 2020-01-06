#include <kernel/devices/tty.h>
#include <stdarg.h>

void tty_ioctl(void *inode, uint32_t cmd, ...)
{
	extern struct tty_s tty;
	va_list ap;

	va_start(ap, cmd);
	switch (cmd)
	{
		case TTY_IOCTL_GETDX:
		{
			int *dx = va_arg(ap, int*);
			*dx = tty.cursor.x * (KERNEL_FONT_REAL_WIDTH + 1);
			break;
		}
		case TTY_IOCTL_GETDY:
		{
			int *dy = va_arg(ap, int*);
			int start = tty.cursor.y;
			int saved_start;
			int line = -1;

			while (++line < DISPLAY_VCHAR_MAX - 1)
			{
				// Update check line.
				saved_start = start;
				start = (start - 1 < 0) ? tty.cursor.max.y : start - 1;

				// Check if the line existe.
				if (tty.buffer[start][0] == '\0')
				{
					start = saved_start;
					break;
				}
			}
			*dy = line * (KERNEL_FONT_REAL_HEIGHT + 1);
			break;
		}
	}
	va_end(ap);
}
