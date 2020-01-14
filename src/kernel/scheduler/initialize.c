#include <kernel/scheduler.h>

// Internal task queue
struct sched_task sched_task_cache[SCHED_TASK_NB_MAX];
struct sched_task *sched_task_current;
struct sched_task *sched_task_queue;
struct process *process_current;

void sched_initialize(void)
{
	// Initialize internal scheduler cache
	for (int i = 0 ; i < SCHED_TASK_NB_MAX ; i = i + 1)
	{
		sched_task_queue[i].status = SCHED_DOWN;
		sched_task_queue[i].process = NULL;
		sched_task_queue[i].priority._static = 0;
		sched_task_queue[i].priority._dynamic = 0;
		sched_task_queue[i].next = NULL;
	}

	// Initialize internal process informations
	// @note:
	// * process_sched_current	Used by the kernel to handle
	// 				interrupt / exception
	// * process_user_current	Used by the kernel to allow
	// 				interaction with the Video RAM
	// 				and the screen driver.
	// TODO: use internal vram for each processus ?
	sched_task_current = NULL;
	sched_task_queue = NULL;
	process_current = NULL;
}
