#include <kernel/scheduler.h>
#include <kernel/util/atomic.h>

struct process *process_get(pid_t pid)
{
	extern struct process *proc_table;
	struct process *proc;

	// Check error
	if (pid <= 0)
		return (NULL);

	// Start atomic operation
	atomic_start();

	// Proess table walk
	proc = proc_table;
	while (proc != NULL)
	{
		// Check target process
		if (proc->pid != pid) {
			proc = proc->next;
			continue;
		}
		
		// Check if the process is alive
		// @note: we use 0xdeadbeef to determine
		// the process state
		if (proc->parent == (void*)0xdeadbeef)
			proc = NULL;

		// Stop atomic operations
		atomic_stop();
		return (proc);
	}

	// Stop atomic operation
	atomic_stop();
	return (NULL);
}
