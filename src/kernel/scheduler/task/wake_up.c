#include <kernel/scheduler.h>
#include <kernel/util/atomic.h>

void sched_task_wake_up(struct process *process)
{
	atomic_start();
	process->sched_task->status = SCHED_TASK_RUNNING;
	atomic_stop();
}
