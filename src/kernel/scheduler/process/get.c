#include <kernel/process.h>

struct process *process_get_proc(pid_t pid)
{
	extern struct process_stack process_stack[PROCESS_MAX];

	// Check error
	if (pid < 0 || pid >= PROCESS_MAX)
		return (NULL);

	// Return process.
	return (&process_stack[pid].process);
}

pid_t process_get_pid(struct process *target)
{
	extern struct process_stack process_stack[PROCESS_MAX];
	int i;

	i = -1;
	while (++i < PROCESS_MAX)
	{
		if (&process_stack[i].process == target)
			return (i);
	}
	return (-1);
}
