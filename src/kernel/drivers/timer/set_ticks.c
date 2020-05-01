#include <kernel/drivers/timer.h>
#include <kernel/hardware/tmu.h>
#include <kernel/util/atomic.h>

int timer_set_ticks(int timerID, uint32_t ticks)
{
	extern struct timer_cache_s timercache[TIMER_NUMBER];
	uint8_t status;

	// Check bad ID
	if (timerID < 0 || timerID >= TIMER_NUMBER)
		return (-1);

	// Start Atomic operation.
	atomic_start();

	// Save current timer status (start / stop)
	status = SH7305_TMU.TSTR.BYTE &= 1 << timerID;	// save
	SH7305_TMU.TSTR.BYTE &= ~(1 << timerID);	// stop

	// Setup Timer timer
	SH7305_TMU.TIMER[timerID].TCOR = ticks;
	SH7305_TMU.TIMER[timerID].TCNT = ticks;

	// Start hardware timer.
	SH7305_TMU.TSTR.BYTE |= status;

	// Stop atomic operation and return.
	atomic_stop();
	return (0);
}
