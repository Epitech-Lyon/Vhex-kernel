#include <kernel/drivers/timer.h>
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

	// Stop atomic operations
	atomic_stop();
}
