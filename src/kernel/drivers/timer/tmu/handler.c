#include <kernel/hardware/tmu.h>
#include <kernel/drivers/timer.h>

void sh7305_tmu_handler(void)
{
	extern struct timer_cache_s timercache[TIMER_NUMBER];
	uint32_t intevt;
	int timer_ID;

	// Get timer ID
	// @note: tricky way to get timer ID
	// TODO: find better way !
	intevt = *(uint32_t*)0xff000028;
	timer_ID = (intevt - 0x400) / 0x20;

	// Clear interrupt flags.
	SH7305_TMU.TIMER[timer_ID].TCR.UNF = 0;

	// Execute callback if possible.
	if (timercache[timer_ID].status != TIMER_UNUSED &&
			timercache[timer_ID].callback != NULL) {
		((void (*)(volatile void *))timercache[timer_ID].callback)(timercache[timer_ID].arg);
	}

}
