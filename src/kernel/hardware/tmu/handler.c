#include <kernel/hardware/tmu.h>
#include <kernel/devices/timer.h>

void timer_handler(void)
{
	extern struct timer_cache_s timercache[TIMER_NUMBER];
	uint32_t intevt;
	int timer_ID;

	// Get timer ID
	// @note: tricky way to get timer ID
	// TODO: find better way !
	intevt = *(uint32_t*)0xff000028;
	timer_ID = (intevt - 0x400) >> 9;

	// Stop timer flag and clear interrupt flags.
	SH7305_TMU.TSTR.BYTE &= ~(1 << timer_ID);
	SH7305_TMU.TIMER[timer_ID].TCR.UNF = 0;

	// Execute callback if possible.
	if (timercache[timer_ID].status != TIMER_UNUSED &&
			timercache[timer_ID].callback != NULL)
	{
		((void (*)(volatile void *))timercache[timer_ID].callback)(timercache[timer_ID].arg);
	}

	// Re-start timer.
	SH7305_TMU.TSTR.BYTE |= 1 << timer_ID;
}
