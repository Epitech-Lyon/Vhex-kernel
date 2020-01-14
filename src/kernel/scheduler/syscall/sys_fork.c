/*#include <kernel/process.h>

pid_t sys_fork(void)
{
	extern process_t *process_current;
	process_t **process_new;
	pid_t process_pid;

	// Get the new process slot.
	process_new = &process_current->child;
	while (*process_new != NULL)
		process_new = &(*process_new)->next;

	// Try to find new sheduler place.
	process_pid = process_alloc(&(*process_new));
	if (*process_new == NULL)
		return (-1);
	
	// Initialize context.
	return (-1);
}*/
