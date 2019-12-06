#include <kernel/devices/display.h>
#include <kernel/syscall.h>

// Create VRAM global.
uint8_t *vram = NULL;

int display_open(void)
{
	//TODO: handle gxcg50 !!!
	vram = casio_Bdisp_GetVRAM();
	return (0);
}
