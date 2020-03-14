#include <kernel/context.h>
#include <kernel/hardware/tmu.h>
#include <kernel/devices/earlyterm.h>

// Test
uint32_t counter = 0;

void sched_debug(common_context_t *context_current, common_context_t *context_next)
{
	extern uint32_t sched_timer_id;
	extern uint32_t sched_timer_address;

	uint32_t tstr = SH7305_TMU.TSTR.BYTE;
	uint32_t tcr = *(uint16_t*)sched_timer_address;
	uint32_t tcor = *(uint32_t*)(sched_timer_address - 4);
	uint32_t tcnt = *(uint32_t*)(sched_timer_address - 8);

	
	//SH7305_TMU.TSTR.STR2 = 0;
	//SH7305_TMU.TSTR.STR1 = 0;
	//SH7305_TMU.TSTR.STR0 = 0;

	//SH7305_TMU.TIMER[0].TCR.UNF = 0;
	//SH7305_TMU.TIMER[1].TCR.UNF = 0;
	//SH7305_TMU.TIMER[2].TCR.UNF = 0;

	// Display internal data
	earlyterm_write("Scheduler_schudele !");
	earlyterm_write("context current = %p", context_current);
	earlyterm_write("context next = %p", context_next);
	earlyterm_write("counter = %#x", counter++);
	earlyterm_write("TSTR: %#x", tstr);
	earlyterm_write("TCOR: %#x", tcor);
	earlyterm_write("TCNT: %#x", tcnt);
	earlyterm_write("TCR:  %#x", tcr);
}
