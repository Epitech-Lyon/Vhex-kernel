#include <kernel/scheduler.h>
#include <kernel/util/atomic.h>

static struct sched_task *sched_alloc(void)
{
	extern struct sched_task sched_task_cache[SCHED_TASK_NB_MAX];
	int i;

	i = SCHED_TASK_NB_MAX;
	while (--i >= 0)
	{
		if (sched_task_cache[i].process == NULL)
			return (&sched_task_cache[i]);
	}
	return (NULL);
}

int sched_task_add(struct process *process)
{
	extern struct sched_task *sched_task_queue;
	struct sched_task *sched_slot;

	// Check error
	if (process == NULL)
		return (-1);

	// Start atomic operations
	atomic_start();

	// Try to find free slot
	sched_slot = sched_alloc();
	if (sched_slot == NULL)
	{
		atomic_stop();
		return (-2);
	}
	
	// Update internal process data
	process->sched_task = sched_slot;

	// Initialize new task
	// TODO: update quantum ticks management
	sched_slot->priority._static = SCHED_QUANTUM_TICKS;
	sched_slot->priority._dynamic = 0;
	sched_slot->process = process;
	sched_slot->status = SCHED_TASK_RUNNING;

	// Register task into scheduler task queue
	sched_slot->next = sched_task_queue;
	sched_task_queue = sched_slot;

	// Stop atomic operation
	atomic_stop();
	return (0);
}
