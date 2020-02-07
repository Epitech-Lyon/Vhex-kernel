#include <kernel/util.h>
#include <kernel/atomic.h>

// Internal symbols
static volatile uint8_t *sel = (void *)0xb4000000;
static volatile uint8_t *cmd = (void *)0xb4010000;

static void command(uint8_t reg, uint8_t data)
{
	*sel = reg;
	*cmd = data;
}

void t6k11_variant_lcd_driver(uint8_t *vram)
{
	int y;

	// Start atomic operations
	atomic_start();

	y = -1;
	while (++y < 64)
	{
		// Row selector
		command(8, y | 0x80);
		command(8, 4);

		// Send the row's data to the device
		// @note: Unroll the loop for more speed (?)
		*sel = 10;
		*cmd = vram[0];
		*cmd = vram[1];
		*cmd = vram[2];
		*cmd = vram[3];

		*cmd = vram[4];
		*cmd = vram[5];
		*cmd = vram[6];
		*cmd = vram[7];

		*cmd = vram[8];
		*cmd = vram[9];
		*cmd = vram[10];
		*cmd = vram[11];

		*cmd = vram[12];
		*cmd = vram[13];
		*cmd = vram[14];
		*cmd = vram[15];

		// Update vram ptr
		vram = &vram[16];
	}

	// Stop atomic operations
	atomic_stop();
}
