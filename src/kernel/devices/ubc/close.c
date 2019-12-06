#include <kernel/devices/ubc.h>
#include <kernel/hardware/power.h>

int ubc_close(void)
{
	//FIXME: check MPU before call this function !!!
	//FIXME: Disable all chennel before power OFF ?
	SH7305_POWER.MSTPCR0.UBC = 1;
	return (1);
}
