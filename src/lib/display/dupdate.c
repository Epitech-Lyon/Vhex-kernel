#include <lib/display.h>
#include <kernel/devices/display.h>
#include <kernel/hardware/t6k11.h>

void dupdate(void)
{
	t6k11_display(vram);
}
