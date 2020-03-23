#include <kernel/scheduler.h>

//TODO: assembly !
// @note: This part *SHOULD* be exeption safe !
int sched_schedule(common_context_t **context_current, common_context_t **context_next)
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
		if (task_next->process->status == PROC_RUNNING)
			break;

		// Get next task
		task_next = task_next->next;
		if (task_next == NULL)
			task_next = sched_task_queue;
	}

	// Check error
	if (task_next == sched_task_current)
		return (-1);

	// Get contexts
	*context_current = NULL;
	if (sched_task_current != NULL)
		*context_current = &sched_task_current->process->context;
	*context_next = &task_next->process->context;

	// Update scheduler task / process current
	sched_task_current = task_next;
	process_current = sched_task_current->process;
	return (0);


	// Check current task
	// TODO: check process status !!
/*	if (sched_task_current == NULL)
	{
		task_current = NULL;
		task_next = (sched_task_queue != NULL) ? sched_task_queue : NULL;
	} else {
		task_current = sched_task_current;
		task_next = (sched_task_current->next != NULL)
			? sched_task_current->next
			: sched_task_queue;
	}

	// Check potantial error
	if (task_next == NULL || task_next == sched_task_current)
		return (-1);
	
	// Update internal scheduler task cursor
	sched_task_current = task_next;
	process_current = sched_task_current->process;

	// Get context
	*context_current = (task_current != NULL) ? &task_current->process->context : NULL;
	*context_next = &task_next->process->context;
*/
	// DEBUG !
	/*kvram_clear();
	printk(0, 0, "Scheduler_schudele !");
	printk(0, 1, "task current = %p", task_current);
	printk(0, 2, "task next = %p", task_next);
	printk(0, 3, "context current = %p", *context_current);
	printk(0, 4, "context next = %p", *context_next);
	kvram_display();
	DBG_WAIT;*/
	//return (0);
}
