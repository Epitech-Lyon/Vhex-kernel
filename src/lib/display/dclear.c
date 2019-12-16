#include <lib/display.h>

// Internal VRAM address.
uint32_t vram[256];

void dclear(void)
{
	int i;

	i = 256;
	while (--i >= 0)
		vram[i] = 0x00000000;
}
