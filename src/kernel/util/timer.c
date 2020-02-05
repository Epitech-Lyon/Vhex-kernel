#include <kernel/util.h>
#include <kernel/hardware/tmu.h>
#include <kernel/atomic.h>

// Internal timer "cache".
struct timer_cache_s timercache[TIMER_NUMBER];

__attribute__((constructor))
void timer_constructor(void)
{
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
}

int timer_install(void *callback, void *arg, uint32_t ticks, uint8_t mode)
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

	// Debug !!
	/*kvram_clear();
	printk(0, 0, "Timer find: %d\n", timer_ID);
	printk(0, 1, "TMU = %p\n", &SH7305_TMU.TIMER[timer_ID]);
	kvram_display();
	DBG_WAIT;*/

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
	if (mode & 1)
		SH7305_TMU.TSTR.BYTE |= 1 << timer_ID;

	// End of atomic operations.
	// and return the timer ID.
	atomic_stop();
	return (timer_ID);
}

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

int timer_start(int timer_ID)
{
	extern struct timer_cache_s timercache[TIMER_NUMBER];

	// Check bad ID
	if (timer_ID < 0 || timer_ID >= TIMER_NUMBER)
		return (-1);

	// Start Atomic operation.
	atomic_start();

	// Start hardware timer.
	SH7305_TMU.TSTR.BYTE |= 1 << timer_ID;

	// Stop atomic operation and return.
	atomic_stop();
	return (0);
}
