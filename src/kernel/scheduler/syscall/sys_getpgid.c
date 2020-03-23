#include <kernel/syscall.h>
#include <kernel/scheduler.h>
#include <kernel/util/atomic.h>

pid_t sys_getpgid(void)
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

	// Get Process Groupe ID
	ret = process_current->pgid;

	// Stop atomic operation
	atomic_stop();
	return (ret);
}
