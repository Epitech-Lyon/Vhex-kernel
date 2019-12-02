#include <lib/display.h>

void dclear(void)
{
	int i;

	i = 256;
	while (--i >= 0)
		VRAM[i] = 0x00000000;
}
