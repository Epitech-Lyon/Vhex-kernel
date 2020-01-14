#include <kernel/scheduler.h>

void sched_start(void)
{
	extern struct sched_task *sched_task_queue;
	extern struct process *process_current;

	//TODO: get CPU frequency !
	//TODO: setup TMU0 interrupt !
	process_current = sched_task_queue->process;
	sched_schedule();
}
