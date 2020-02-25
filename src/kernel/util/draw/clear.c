#include <kernel/util/draw.h>
#include <kernel/util/atomic.h>

// Internal Video RAM
uint32_t vram[256];

/* kvram_clear() - Wipe the Video RAM */
void kvram_clear(void)
{
	int i;

	// The Video RAM is shared between each
	// process and the kernel, so we should
	// use atomic operation when we use it.
	atomic_start();

	// Wipe Video RAM
	// @note: here, we suppose that the VRAM is
	// 4-aligned
	i = 256;
	while (--i >= 0)
		vram[i] = 0x00000000;

	// End of atomic operation
	atomic_stop();
}
