#include <lib/display.h>
#include <kernel/syscall.h>

void dupdate(void)
{
	casio_Bdisp_PutDisp_DD();
}
