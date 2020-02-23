#include <kernel/util/draw.h>
#include <kernel/util/atomic.h>

/* kvram_scroll() - Scroll up the Video RAM */
//FIXME: This part is hardware specific (128x64px)!!
void kvram_scroll(int lines)
{
	extern uint32_t vram[256];
	int i;

	// The Video RAM is shared between each
	// process and the kernel, so we should
	// use atomic operation when we use it.
	atomic_start();

	// Scoll n lines
	i = 0;
	while ((i >> 2) < 63 - lines)
	{
		vram[i + 0] = vram[i + (lines << 2) + 0];
		vram[i + 1] = vram[i + (lines << 2) + 1];
		vram[i + 2] = vram[i + (lines << 2) + 2];
		vram[i + 3] = vram[i + (lines << 2) + 3];
		i = i + 4;
	}
	// Clear last n lines
	while ((i >> 2) < 64)
	{
		vram[i + 0] = 0x00000000;
		vram[i + 1] = 0x00000000;
		vram[i + 2] = 0x00000000;
		vram[i + 3] = 0x00000000;
		i = i + 4;
	}

	// End of atomic operation
	atomic_stop();
}
