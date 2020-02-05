#include <kernel/process.h>

// This function SHOULD NOT be called
// without atomic operation !!
// TODO: dynamic allocation ?
pid_t process_alloc(struct process **process)
{
	extern struct process_stack process_stack[PROCESS_MAX];

	for (int i = 0 ; i < PROCESS_MAX ; i = i + 1)
	{
		if (process_stack[i].status == PROC_UNUSED)
		{
			process_stack[i].status = PROC_USED;
			*process = &process_stack[i].process;
			return (i);
		}
	}
	return (-1);
}
