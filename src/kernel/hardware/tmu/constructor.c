#include <kernel/hardware/tmu.h>
#include <kernel/devices/timer.h>

// Internal timer "cache".
struct timer_cache_s timercache[TIMER_NUMBER];

__attribute__((constructor))
void timer_constructor(void)
{
	for (int i = 0 ; i < TIMER_NUMBER ; i = i + 1)
	{
		timercache[i].status = TIMER_UNUSED;
		timercache[i].callback = NULL;
		timercache[i].arg = NULL;
	}
}
