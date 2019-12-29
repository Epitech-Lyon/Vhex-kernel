#include <kernel/process.h>
#include <lib/string.h>

// This function SHOULD not be called
// without atomic operation !!
pid_t process_alloc(process_t **process)
{
	extern struct process_stack_s process_stack[PROCESS_MAX];

	for (int i = 0 ; i < PROCESS_MAX ; i = i + 1)
	{
		if (process_stack[i].status == PROC_IDLE)
		{
			*process = &process_stack[i].process;
			return (i);
		}
	}
	return (-1);
}
