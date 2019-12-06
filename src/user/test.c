#include <kernel/syscall.h>

void test(void)
{
	uint8_t *vram;

	vram = casio_Bdisp_GetVRAM();
}
