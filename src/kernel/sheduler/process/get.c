#include <kernel/process.h>

struct process *process_get(pid_t pid)
{
	extern struct process_stack process_stack[PROCESS_MAX];

	// Check error
	if (pid < 0 || pid >= PROCESS_MAX)
		return (NULL);

	// Return process.
	return (&process_stack[pid].process);
}
