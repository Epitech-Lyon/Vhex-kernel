#include <kernel/drivers/timer.h>
#include <kernel/hardware/tmu.h>
#include <kernel/util/atomic.h>

/*
** int timer_instal(void*, void*, tmode_t)
**
** @info
** This function will try to setup an hardware timer.
*/
int timer_install(void *callback, void *arg, uint32_t ticks, tmode_t mode)
{
	extern struct timer_cache_s timercache[TIMER_NUMBER];
	int timer_ID;

	// Do atomic operation because of
	// sheared timer cache and I/O access.
	atomic_start();

	// Find free timer.
	timer_ID = -1;
	while (++timer_ID < TIMER_NUMBER &&
			timercache[timer_ID].status != TIMER_UNUSED);
	if (timer_ID >= TIMER_NUMBER)
	{
		atomic_stop();
		return (-1);
	}

	// Initialise internal timer data.
	timercache[timer_ID].callback = callback;
	timercache[timer_ID].status = mode;
	timercache[timer_ID].arg = arg;
	
	// Initialise hardware module.
	SH7305_TMU.TSTR.BYTE &= ~(1 << timer_ID);	// Stop timer.

	// Setup Timer
	SH7305_TMU.TIMER[timer_ID].TCOR = ticks;
	SH7305_TMU.TIMER[timer_ID].TCNT = ticks;

	// Configure timer
	SH7305_TMU.TIMER[timer_ID].TCR.UNF = 0;		// Clear interrupt flag.
	SH7305_TMU.TIMER[timer_ID].TCR.UNIE = 1;	// Enable interrupt.
	
	// Start timer if needed.
	if (mode == TIMER_START)
		SH7305_TMU.TSTR.BYTE |= 1 << timer_ID;

	// End of atomic operations.
	// and return the timer ID.
	atomic_stop();
	return (timer_ID);
}
