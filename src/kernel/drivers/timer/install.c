#include <kernel/drivers/timer.h>
#include <kernel/drivers/cpg.h>
#include <kernel/hardware/tmu.h>
#include <kernel/util/atomic.h>

/*
** int timer_instal(void*, void*, tmode_t)
**
** @info
** This function will try to setup an hardware timer.
*/
int timer_install(void *callback, void *arg, int hertz, tmode_t mode)
{
	extern struct timer_cache_s timercache[TIMER_NUMBER];
	int prescaler[8] = {4, 16, 64, 256, 1024, 4, 4, 4};
	uint32_t ticks;
	int timerID;

	// Do atomic operation because of
	// sheared timer cache and I/O access.
	atomic_start();

	// Find free timer.
	timerID = -1;
	while (++timerID < TIMER_NUMBER &&
			timercache[timerID].status != TIMER_UNUSED);
	if (timerID >= TIMER_NUMBER)
	{
		atomic_stop();
		return (-1);
	}

	// Initialise internal timer data.
	timercache[timerID].callback = callback;
	timercache[timerID].status = mode;
	timercache[timerID].arg = arg;
	
	// Initialise hardware module.
	SH7305_TMU.TSTR.BYTE &= ~(1 << timerID);	// Stop timer.

	// Calculate real ticks needed
	// FIXME: correct ticks calculation (remove / 2 ?)
	ticks = cpg_get_frequency(CPG_PERIPHERAL) / 2;
	ticks = ticks /  prescaler[SH7305_TMU.TIMER[timerID].TCR.TPSC];
	ticks = ticks / hertz;

	// Setup Timer
	SH7305_TMU.TIMER[timerID].TCOR = ticks;
	SH7305_TMU.TIMER[timerID].TCNT = ticks;

	// Configure timer
	SH7305_TMU.TIMER[timerID].TCR.UNF = 0;		// Clear interrupt flag.
	SH7305_TMU.TIMER[timerID].TCR.UNIE = 1;		// Enable interrupt.
	
	// Start timer if needed.
	if (mode == TIMER_START)
		SH7305_TMU.TSTR.BYTE |= 1 << timerID;

	// End of atomic operations.
	// and return the timer ID.
	atomic_stop();
	return (timerID);
}
