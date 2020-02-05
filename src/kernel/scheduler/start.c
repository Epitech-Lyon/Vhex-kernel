#include <kernel/scheduler.h>
#include <kernel/hardware/cpg.h>
#include <kernel/hardware/tmu.h>
#include <kernel/util.h>

// Internal data used by the scheduler handler
uint32_t sched_timer_id = 0;
uint32_t sched_timer_address = 0;
uint32_t sched_timer_intevt = 0;
uint32_t sched_timer_tstr_bit = 0;

// Internal private function
//void sched_handler(void);

//FIXME: remove me and wrap timer interrupt when occur !!
static void scheduler_timer_callback(void)
{
	common_context_t *context_current;
	common_context_t *context_next;

	//TODO: quantum handling !!
	//TODO: scheduler block / unblock options !!
	//FIXME: Get SPC, SSR, ... !!
	
	// Get current context and 
	if (sched_schedule(&context_current, &context_next) != 0)
		return;

	// Debug
	//kvram_clear();
	//printk(0, 0, "Scheduler_schudele !");
	//printk(0, 1, "context current = %p", context_current);
	//printk(0, 2, "context next = %p", context_next);
	//kvram_display();
	//printk(0, 3, "context switch !");
	//kvram_display();
	//DBG_WAIT;

	// Context switch	
	sched_context_switch(context_current, context_next);
}

void sched_start(void)
{
	extern struct sched_task *sched_task_queue;
	extern struct process *process_current;
	uint32_t fll_freq;
	uint32_t pll_freq;
	uint32_t cpu_freq;
	uint32_t bus_freq;
	uint32_t per_freq;


	// Calculate FLL frequency (Khz)
	// @note: RCLK = 32 768 Hz
	fll_freq = SH7305_CPG.FLLFRQ.FLF * 32768;		// Hz
	fll_freq = fll_freq /  (1 << SH7305_CPG.FLLFRQ.SELXM);	// Check FLL output division

	// Calculate PLL frequency (Khz)
	pll_freq = fll_freq * (SH7305_CPG.FRQCRA.STC + 1);

	// Calculate CPU clock frequency !
	cpu_freq = pll_freq / (1 << (SH7305_CPG.FRQCRA.IFC + 1));

	// Calculate BUS clock frequency !
	bus_freq = pll_freq / (1 << (SH7305_CPG.FRQCRA.BFC + 1));

	// Calculate Peripheral clock frequency !
	per_freq = pll_freq / (1 << (SH7305_CPG.FRQCRA.PFC + 1));
	

	// Debug
	kvram_clear();
	printk(0, 0,
		"FLL freq: %d.%d Mhz\n"
		"PLL freq: %d.%d Mhz\n"
		"CPU freq: %d.%d Mhz\n"
		"BUS freq: %d.%d Mhz\n"
		"Per freq: %d.%d Mhz",
		fll_freq / 1000000, (((fll_freq - ((fll_freq / 1000000)) * 1000000)) + 999) / 1000,
		pll_freq / 1000000, (((pll_freq - ((pll_freq / 1000000)) * 1000000)) + 999) / 1000,
		cpu_freq / 1000000, (((cpu_freq - ((cpu_freq / 1000000)) * 1000000)) + 999) / 1000,
		bus_freq / 1000000, (((bus_freq - ((bus_freq / 1000000)) * 1000000)) + 999) / 1000,
		per_freq / 1000000, (((per_freq - ((per_freq / 1000000)) * 1000000)) + 999) / 1000
	);
	kvram_display();
	DBG_WAIT;

	// Register first process ! 
	process_current = sched_task_queue->process;

	// Setup TMU0 (scheduler) interrupt !
	// @note: I use Po/4 on TMU prescaler
	// TODO: generate quantum and quantum counter for preemption !
	uint32_t ticks = (per_freq / 4) / 16;
	//sched_timer_id = timer_install(scheduler_timer_callback, NULL, ticks, 0);
	sched_timer_id = timer_install(NULL, NULL, ticks, 0);
	sched_timer_address = (uint32_t)&SH7305_TMU.TIMER[sched_timer_id].TCR; 
	sched_timer_intevt = 0x400 + (0x20 * sched_timer_id);
	sched_timer_tstr_bit = 1 << sched_timer_id;

	// Debug
	kvram_clear();
	printk(0, 0, "timer ID:    %d", sched_timer_id);
	printk(0, 1, "timer addr:  %#x", sched_timer_address);
	printk(0, 2, "timer event: %#x", sched_timer_intevt);
	printk(0, 3, "timer TSTR:  %#x", sched_timer_tstr_bit);
	kvram_display();
	DBG_WAIT;
	

	// Start scheduler timer
	timer_start(sched_timer_id);
}
