#include <kernel/drivers/timer.h>
#include <kernel/hardware/tmu.h>
#include <kernel/util/atomic.h>

// Internal timer "cache".
struct timer_cache_s timercache[TIMER_NUMBER];

/*
** void timer_constructor(void)
**
** @info
** Constructor called during the bootstrap of the
** kernel. Should not be called anymore.
*/
__attribute__((constructor))
void timer_constructor(void)
{
	// Start atomic operations
	atomic_start();

	// Initialise internal cache
	for (int i = 0 ; i < TIMER_NUMBER ; i = i + 1)
	{
		timercache[i].status = TIMER_UNUSED;
		timercache[i].callback = NULL;
		timercache[i].arg = NULL;
	}

	// Configure TMU
	SH7305_TMU.TSTR.STR0 = 0;			// Stop timer 0
	SH7305_TMU.TSTR.STR1 = 0;			// Stop timer 1
	SH7305_TMU.TSTR.STR2 = 0;			// Stop timer 2

	// Configure TMU timers
	for (int i = 0 ; i < 3 ; ++i)
	{
		SH7305_TMU.TIMER[i].TCOR = 0xffffffff;	// Reset timer constant register
		SH7305_TMU.TIMER[i].TCNT = 0xffffffff;	// Reset timer counter
		SH7305_TMU.TIMER[i].TCR.UNF = 0;	// Clear interrupt flags
		SH7305_TMU.TIMER[i].TCR.UNIE = 0;	// Disable timerinterrupt
		SH7305_TMU.TIMER[i].TCR.CKREG = 0b00;	// Count on rising edge
		SH7305_TMU.TIMER[i].TCR.TPSC = 0b000;	// Select P/4 prescaler
	}

	// Stop atomic operations
	atomic_stop();
}
