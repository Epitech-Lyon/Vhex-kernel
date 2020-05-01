#include  <kernel/drivers/timer.h>
#include <kernel/util/atomic.h>

int timer_set_callback(int timerID, void *callback, void *arg)
{
	extern struct timer_cache_s timercache[TIMER_NUMBER];

	// Check bad ID
	if (timerID < 0 || timerID >= TIMER_NUMBER)
		return (-1);

	// Start Atomic operation.
	atomic_start();

	// Set new callback and arg
	timercache[timerID].callback = callback;
	timercache[timerID].arg = arg;

	// Stop atomic operation and return.
	atomic_stop();
	return (0);
}
