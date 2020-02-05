/*#include <kernel/process.h>

pid_t sys_fork(void)
{
	extern struct process *process_current;
	struct process *process_new;
	pid_t process_new_pid;

	// Check error
	if (process_current == NULL)
		return (-1);

	// Get the new process slot.
	new_pid = process_alloc(&process_new);
	if (new_pid != 0)
		return (-1);

	// Try to create user stack
	process_new->memory.stack.size.user = process_current->;
	process_new->memory.stack.user = (uint32_t)pm_alloc(process_new->memory.stack.size.user);
	if (process_new->memory.stack.user == 0x00000000)
	{
		process_free(process_new);
		return (-1);
	}

	// Try to create kernel stack
	process_new->memory.stack.size.kernel = process_current->memory.stack.size.kernel;
	process_new->memory.stack.kernel = (uint32_t)pm_alloc(process_new->memory.stack.size.kernel);
	if (process_new->memory.stack.kernel == 0x00000000)
	{
		pm_free(process_new->memory.stack.user);
		process_free(process_new);
		return (-1);
	}


	//TODO: dump stack data
	//TODO: dump code data
	//TODO: Initialize context.
	return (-1);
}*/
