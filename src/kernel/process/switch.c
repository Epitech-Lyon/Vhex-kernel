#include <kernel/process.h>

//FIXME: atomic operation !!
int process_switch(pid_t pid)
{
	extern process_t *process_current;
	common_context_t *context_current;
	common_context_t *context_next;
	process_t *process;

	// Get current context
	context_current =
		(process_current != NULL)
		? &process_current->context
		: NULL;

	// Get next context.
	process = process_get(pid);
	if (process == NULL)
		return (-1);
	context_next = &process->context;

	// Context switch
	// TODO: SYSCALL !!!!!
	//context_switch(context_current, context_next);
	return (0);
}
