#include <lib/display.h>
#include <kernel/hardware/t6k11.h>

//FIXME: Check OS 3.00 !
void dupdate(void)
{
	t6k11_display(VRAM);
}
