#include <kernel/syscall.h>
#include <kernel/scheduler.h>
#include <kernel/util/atomic.h>

int sys_setpgid(pid_t pid, pid_t pgid)
{
	extern struct process *process_current;
	struct process *proc;

	// Start atomic operations
	atomic_start();

	// Get the appropriate process
	proc = process_current;
	if (pid != 0)
		proc = process_get(pid);

	// Check error
	if (proc == NULL)
		return (-1);

	// Update Proocess Group ID
	proc->pgid = (pgid == 0) ? process_current->pgid : pgid;
	return (0);
}
