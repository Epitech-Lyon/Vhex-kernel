#include <kernel/syscall.h>
#include <kernel/scheduler.h>
#include <kernel/util/atomic.h>

pid_t sys_getppid(void)
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

	// Check first process
	if (process_current->parent == NULL) {
		atomic_stop();
		return (0);
	}

	// Get Parent Process ID
	ret = process_current->parent->pid;

	// Stop atomic operation
	atomic_stop();
	return (ret);
}
