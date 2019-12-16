#include <kernel/devices/display.h>
#include <kernel/syscall.h>

// Create VRAM global.
uint32_t vram[256];

int display_open(void)
{
	//TODO: handle gxcg50 !!!
	return (0);
}
