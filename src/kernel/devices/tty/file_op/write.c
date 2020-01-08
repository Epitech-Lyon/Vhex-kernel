#include <kernel/devices/tty.h>
#include <kernel/devices/display.h>
#include <kernel/atomic.h>
#include <kernel/util.h>

// Internal TTY object.
extern struct tty_s tty;

static void tty_vertical_update(void)
{
	// Get next line.
	tty.cursor.y =
			(tty.cursor.y + 1 <= tty.cursor.max.y)
			? tty.cursor.y + 1
			: 0;

	// Wipe new line.
	memset(tty.buffer[tty.cursor.y], '\0', tty.cursor.max.x);
}

static int tty_horizontal_update(void)
{
	tty.cursor.x = tty.cursor.x + 1;
	if (tty.cursor.x >= tty.cursor.max.x)
	{
		tty_vertical_update();
		tty.cursor.x = 0;
		return (1);
	}
	return (0);
}

// Line discipline :D
static ssize_t tty_buffer_update(const uint8_t *buffer, size_t count)
{
	int16_t offset;
	ssize_t i;

	i = -1;
	while (++i < (ssize_t)count)
	{
		// Check bell char.
		if (buffer[i] == '\a')
		{
			// TODO
			continue;
		}
		// Check backspace.
		if (buffer[i] == '\b')
		{
			if (tty.cursor.x > 0)
				tty.cursor.x = tty.cursor.x - 1;
			tty.buffer[tty.cursor.y][tty.cursor.x] = '\0';
			continue;
		}

		// Check horizotal tab.
		if (buffer[i] == '\t')
		{
			// Check if we need a new line or not.
			offset = 5 - (tty.cursor.x - ((tty.cursor.x / 5) * 5));
			if (tty.cursor.x + offset < tty.cursor.max.x)
			{
				memset(&tty.buffer[tty.cursor.y][tty.cursor.x], ' ', offset);
				tty.cursor.x = tty.cursor.x + offset;
				continue;
			}

			// If a new line is required char.
			// Generate a new line.
			tty.cursor.x = 0;
			tty_vertical_update();
			continue;
		}

		// Check new line char.
		if (buffer[i] == '\n')
		{
			tty.cursor.x = 0;
			tty_vertical_update();
			continue;
		}

		
		// Check 'form feed' and 'vertical tab' char.
		// @note: for now this character is interpreted like '\v'
		if (buffer[i] == '\f' || buffer[i] == '\v')
		{
			tty_vertical_update();
			memset(tty.buffer[tty.cursor.y], ' ', tty.cursor.x);
			continue;
		}

		// Check carriage return.
		if (buffer[i] == '\r') { tty.cursor.x = 0; continue;}

		// Update TTY buffer char.
		tty.buffer[tty.cursor.y][tty.cursor.x] = buffer[i];
		tty_horizontal_update();
	}
	return (i);
}

// FIXME: this function is device-specific !
// TODO: Update me ?
static void tty_display(void)
{
	int saved_start;
	int line_len;
	int row_last;
	int scolumn;
	int srow;
	int line;
	int start;
	int i;

	// Start atomic operation.
	atomic_start();

	// Save display "context".
	display_ioctl(DISPLAY_IOCTL_GETY, &srow);
	display_ioctl(DISPLAY_IOCTL_GETX, &scolumn);

	// Clear Display
	// TODO: IOCTL to disable auto put on screen.
	display_ioctl(DISPLAY_IOCTL_CLEAR);

	// Get the "first" line and number of line.
	// @note: circular buffer.
	line = 0;
	start = tty.cursor.y;
	while (++line < DISPLAY_VCHAR_MAX)
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

	// Display "on-screen" string lines.
	i = -1;
	while (++i < line)
	{
		// Set display cursor.
		display_ioctl(DISPLAY_IOCTL_SETX, 0);
		display_ioctl(DISPLAY_IOCTL_SETY, i);

		// Get / check line lenght.
		line_len = strnlen(tty.buffer[start], tty.cursor.max.x);
		if (line_len == 0)
			continue;

		// Display line.
		tty.primitives.write(tty.buffer[start], line_len);

		// Get "next" line.
		start = (start + 1 > tty.cursor.max.y) ? 0 : start + 1;
	}

	// Display on screen.
	display_ioctl(DISPLAY_IOCTL_DISPLAY);

	// Restore display "context"
	display_ioctl(DISPLAY_IOCTL_SETX, scolumn);
	display_ioctl(DISPLAY_IOCTL_SETY, srow);

	// End atomic operation
	atomic_stop();
}

ssize_t tty_write(void *inode, const void *buffer, size_t count)
{
	ssize_t written;

	// Uopdate internal buffer and display
	// TTY on the screen.
	written = tty_buffer_update(buffer, count);
	tty_display();

	// Return the number of char written into
	// TTY's internal buffer.
	return (written);
}
