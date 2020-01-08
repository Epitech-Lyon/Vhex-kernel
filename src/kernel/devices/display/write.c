#include <kernel/devices/display.h>
#include <kernel/hardware/t6k11.h>
#include <kernel/atomic.h>

// Internal display cursor.
int dcurx = 0;
int dcury = 0;

static void display_clear(void)
{
	int i;

	i = 256;
	while (--i >= 0)
		vram[i] = 0x00000000;
}

static void display_ascii(int column, int row, char c)
{
	int x;
	int y;

	// Get real X / Y positions.
	y = row * (KERNEL_FONT_REAL_HEIGHT + 1);
	x = column * (KERNEL_FONT_REAL_WIDTH + 1);

	// Draw ASCII character.
	font_draw(x, y, c);
}

ssize_t display_write(const void *buffer, size_t count)
{
	ssize_t i;
	int x;
	int y;
	// Start atomic operation
	atomic_start();

	// Write ASCII charactere.
	y = 0;
	x = 0;
	i = -1;
	while (++i < (ssize_t)count)
	{
		// Draw ASCII character.
		display_ascii(x + dcurx, y + dcury, ((uint8_t*)buffer)[i]);

		// Update draw position.
		x = x + 1;
		if (x + dcurx >= DISPLAY_SCREEN_WIDTH)
		{
			x = 0;
			y = y + 1;
			if (y + dcury >= DISPLAY_SCREEN_HEIGHT)
				break;
		}
	}

	// Hardware-specifique screen driver.
	// TODO: Handle OS 3.00 !
	//t6k11_display(vram);

	// End atomic operation.
	atomic_stop();

	// Return the number of written char.
	return (i);
}
