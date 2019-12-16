#include <lib/timer.h>
#include <kernel/hardware/tmu.h>
#include <kernel/devices/timer.h>
#include <kernel/atomic.h>
#include <lib/display.h>

int timer_install(void *callback, void *arg, uint32_t delay_ms, uint8_t mode)
{
	extern struct timer_cache_s timercache[TIMER_NUMBER];
	int timer_ID;

	// Do atomic operation because of
	// sheared timer cache.
	atomic_start();

	// Find free timer.
	timer_ID = -1;
	while (++timer_ID < TIMER_NUMBER &&
			timercache[timer_ID].status != TIMER_UNUSED);
	if (timer_ID >= TIMER_NUMBER)
		return (-1);


	// Initialise internal timer data.
	timercache[timer_ID].callback = callback;
	timercache[timer_ID].status = mode;
	timercache[timer_ID].arg = arg;
	
	// Initialise hardware module.
	SH7305_TMU.TSTR.BYTE &= ~(1 << timer_ID);	// Stop timer.

	// Convert delay in millisecond into timer tick.
	// TODO !!
	SH7305_TMU.TIMER[timer_ID].TCOR = delay_ms * 2000;
	SH7305_TMU.TIMER[timer_ID].TCNT = delay_ms * 2000;
	SH7305_TMU.TIMER[timer_ID].TCR.TPSC = 0b000;	// TODO use appropriate divisor !

	SH7305_TMU.TIMER[timer_ID].TCR.UNF = 0;		// Clear interrupt flag.
	SH7305_TMU.TIMER[timer_ID].TCR.UNIE = 1;	// Enable interrupt.
	
	// Start timer if needed.
	if (mode & 1)
		SH7305_TMU.TSTR.BYTE |= 1 << timer_ID;

	// End of atomic operations.
	// and return the timer ID.
	atomic_end();
	return (timer_ID);
}
