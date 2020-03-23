#include <kernel/syscall.h>
#include <kernel/scheduler.h>
#include <kernel/util/atomic.h>

pid_t sys_getpid(void)
{
	extern struct process *process_current;
	pid_t ret;

	// Start artomic opetations
	atomic_start();

	// Check error
	if (process_current == NULL) {
		atomic_stop();
		return (-1);
	}

	// Get Process ID
	ret = process_current->pid;

	// Stop atomic operation
	atomic_stop();
	return (ret);
}
