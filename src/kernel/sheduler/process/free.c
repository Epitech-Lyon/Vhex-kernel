#include <kernel/process.h>

int process_free(struct process *process)
{
	extern struct process_stack process_stack[PROCESS_MAX];

	for (int i = 0 ; i < PROCESS_MAX ; i = i + 1)
	{
		if (&process_stack[i].process == process)
		{
			process_stack[i].status = PROC_UNUSED;
			return (0);
		}
	}
	return (-1);
}
