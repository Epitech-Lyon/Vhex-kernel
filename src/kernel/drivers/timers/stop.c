#include <kernel/drivers/timer.h>
#include <kernel/hardware/tmu.h>
#include <kernel/util/atomic.h>

/*
** int timer_stop(int timer_ID)
**
** @info
** This function will try to stop hardware timer.
*/
int timer_stop(int timer_ID)
{
	extern struct timer_cache_s timercache[TIMER_NUMBER];

	// Check bad ID
	if (timer_ID < 0 || timer_ID >= TIMER_NUMBER)
		return (-1);

	// Start Atomic operation.
	atomic_start();

	// Start hardware timer.
	SH7305_TMU.TSTR.BYTE &= ~(1 << timer_ID);

	// Stop atomic operation and return.
	atomic_stop();
	return (0);
}
