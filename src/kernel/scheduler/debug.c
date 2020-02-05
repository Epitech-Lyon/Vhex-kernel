#include <kernel/context.h>
#include <kernel/hardware/tmu.h>
#include <kernel/util.h>

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

	kvram_clear();
	printk(0, 0, "Scheduler_schudele !");
	printk(0, 1, "context current = %p", context_current);
	printk(0, 2, "context next = %p", context_next);
	printk(0, 3, "counter = %#x", counter++);
	printk(0, 4, "TSTR: %#x", tstr);
	printk(0, 5, "TCOR: %#x", tcor);
	printk(0, 6, "TCNT: %#x", tcnt);
	printk(0, 7, "TCR:  %#x", tcr);
	kvram_display();
}
