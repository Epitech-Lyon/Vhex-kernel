#include <kernel/scheduler.h>
#include <kernel/hardware/cpg.h>
#include <kernel/hardware/tmu.h>
#include <kernel/devices/earlyterm.h>
#include <kernel/drivers/timer.h>
#include <kernel/drivers/cpg.h>

// Internal data used by the scheduler handler
int sched_timer_id = 0;
uint32_t sched_timer_address = 0;
uint32_t sched_timer_intevt = 0;
uint32_t sched_timer_tstr_bit = 0;

// TODO: generate quantum and quantum counter for preemption !
void sched_start(void)
{
	extern struct sched_task *sched_task_queue;
	extern struct process *process_current;

	// Register first process ! 
	process_current = sched_task_queue->process;

	// Setup TMU0 (scheduler) interrupt !
	sched_timer_id = timer_install(NULL, NULL, 32, TIMER_STOP);
	if (sched_timer_id < 0) {
		earlyterm_write("scheduler FATAL error (timer)\n");
		earlyterm_write("wait manuall reset...");
		while (1);
	}

	// Setup appropriate ticks timing fior the timer
	// @note: I use Po/4 on TMU prescaler and I want 32 process switch per second
	//timer_set_ticks(sched_timer_id, (cpg_get_frequency(CPG_PERIPHERAL) / 4) / 32);
	

	// Initialize internal precalculated data wich will be used
	// when scheduler timer interrupt occur.
	sched_timer_address = (uint32_t)&SH7305_TMU.TIMER[sched_timer_id].TCR; 
	sched_timer_intevt = 0x400 + (0x20 * sched_timer_id);
	sched_timer_tstr_bit = 1 << sched_timer_id;

	// Debug
	//earlyterm_write("* timer ID:    %d\n", sched_timer_id);
	//earlyterm_write("* timer addr:  %#x\n", sched_timer_address);
	//earlyterm_write("* timer event: %#x\n", sched_timer_intevt);
	//earlyterm_write("* timer TSTR:  %#x\n", sched_timer_tstr_bit);
	//DBG_WAIT;

	// Start scheduler timer
	timer_start(sched_timer_id);
}
