#include <lib/display.h>

void dclear(void)
{
	int i;

	i = 1024;
	while (--i >= 0)
		vram[i] = 0x00000000;
}
