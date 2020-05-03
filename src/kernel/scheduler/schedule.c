#include <kernel/scheduler.h>
#include <kernel/devices/earlyterm.h>
#include <kernel/process.h>
#include <kernel/signal.h>

//TODO: assembly ?
//TODO: preemptive handling !!
// @note: This part *SHOULD* be exeption safe !
int sched_schedule(struct cpu_context **context_current, struct cpu_context **context_next)
{
	extern struct sched_task *sched_task_current;
	extern struct sched_task *sched_task_queue;
	extern struct process *process_current;
	struct sched_task *task_next;

	// Get the potential next process context
	task_next = sched_task_queue;
	if (sched_task_current != NULL && sched_task_current->next != NULL)
		task_next = sched_task_current->next;

	// Check / update next task
	while (task_next != sched_task_current)
	{
		// Check process validity
		if (task_next->status == SCHED_TASK_RUNNING
				&& signal_deliver_pending((void*)task_next->process) == 0)
			break;

		// Get next task
		task_next = task_next->next;
		if (task_next == NULL)
			task_next = sched_task_queue;
	}

	// Check error
	// FIXME: check terminate signal ?!
	if (task_next == sched_task_current) {
		if (task_next != NULL)
			signal_deliver_pending((void*)task_next->process);
		return (-1);
	}

	// Get contexts
	*context_current = NULL;
	if (sched_task_current != NULL)
		*context_current = &sched_task_current->process->context;
	*context_next = &task_next->process->context;

	// Update scheduler task / process current
	sched_task_current = task_next;
	process_current = sched_task_current->process;
	return (0);
}
