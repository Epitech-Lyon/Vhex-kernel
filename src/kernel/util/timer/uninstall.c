#include <kernel/util/timer.h>
#include <kernel/hardware/tmu.h>
#include <kernel/util/atomic.h>

/*
** int timer_instal(int timer_ID)
**
** @info
** This function will uninstall hardware timer.
** After this function, the timer ID should not be used.
*/
int timer_uninstall(int timer_ID)
{
	extern struct timer_cache_s timercache[TIMER_NUMBER];

	// Check bad ID
	if (timer_ID < 0 || timer_ID >= TIMER_NUMBER)
		return (-1);

	// Start Atomic operation.
	atomic_start();

	// Stop Hardware timer.
	SH7305_TMU.TSTR.BYTE &= ~(1 << timer_ID);

	// Uninit hardware timer.
	SH7305_TMU.TIMER[timer_ID].TCR.UNF = 0;		// Clear interrupt flag.
	SH7305_TMU.TIMER[timer_ID].TCR.UNIE = 0;	// Disable interrupt.
	SH7305_TMU.TIMER[timer_ID].TCOR = 0xffffffff;
	SH7305_TMU.TIMER[timer_ID].TCNT = 0xffffffff;

	// Free'd timer cache.
	timercache[timer_ID].status = TIMER_UNUSED;
	timercache[timer_ID].callback = NULL;
	timercache[timer_ID].arg = NULL;

	// Stop atomic operation and return.
	atomic_stop();
	return (0);
}
