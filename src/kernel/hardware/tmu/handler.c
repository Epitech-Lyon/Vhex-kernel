#include <kernel/hardware/tmu.h>
#include <kernel/util/timer.h>

void timer_handler(void)
{
	extern struct timer_cache_s timercache[TIMER_NUMBER];
	//static uint32_t counter = 0;
	uint32_t intevt;
	int timer_ID;

	// Get timer ID
	// @note: tricky way to get timer ID
	// TODO: find better way !
	intevt = *(uint32_t*)0xff000028;
	timer_ID = (intevt - 0x400) / 0x20;

	/*if (intevt != 0x400)
	{
		kvram_clear();
		printk(0, 0, "Timer ID: %d\n", timer_ID);
		printk(0, 1, "Interrupt: %#x\n", intevt);
		printk(0, 2, "counter: %d\n", counter++);
		kvram_display();
	}*/

	// Clear interrupt flags.
	SH7305_TMU.TIMER[timer_ID].TCR.UNF = 0;

	// Execute callback if possible.
	if (timercache[timer_ID].status != TIMER_UNUSED &&
			timercache[timer_ID].callback != NULL)
	{
		((void (*)(volatile void *))timercache[timer_ID].callback)(timercache[timer_ID].arg);
	}

}
