#include <kernel/process.h>
#include <kernel/devices/earlyterm.h>
#include <kernel/util/atomic.h>

pid_t sys_waitpid(pid_t pid, int *wstatus, int options)
{
	extern struct process *process_current;
	struct process **proc;
	pid_t ret;


	// Check useless wait
	if (process_current->child == NULL) {
		earlyterm_write("waitpid: no child\n");
		DBG_WAIT;
		return (-1);
	}

	//TODO: handle options
	(void)options;

	// Wait util one of its children specified by PID
	// has changed state
	while (1)
	{
		// Start atomic operation
		atomic_start();

		// Find childs to check
		proc = &process_current->child;
		while (*proc != NULL)
		{
			// Check terminated child
			if ((*proc)->status == PROC_ZOMBIE &&
				((pid < -1 && (*proc)->pgid == -pid) ||
				pid == -1 ||
				(pid == 0 && (*proc)->pgid == process_current->pgid) ||
				(pid > 0 && (*proc)->pid == pid)))
			{
				// Change the child process state
				(*proc)->status = PROC_DEAD;

				// Get the child process PID and stat_loc
				ret = (*proc)->pid;
				*wstatus = (*proc)->__stat_loc;

				// Remove process form the child list
				*proc = (*proc)->sibling;

				//DEBUG
				//earlyterm_write("waitpid: find %d\n", ret);
				//DBG_WAIT;
				//DBG_WAIT;

				// Stop atomic operation and return
				atomic_stop();
				return (ret);
			}

			// Get next child process
			proc = &(*proc)->sibling;
		}

		// Force process to sleep
		// TODO:
		// Schedule !! and change the niciess of the
		// process !!
		//
		// Stop atomic operation and wait
		atomic_stop();
		__asm__ volatile ("sleep; nop");
	}

	// Error
	//earlyterm_write("waitpid: error\n");
	//DBG_WAIT;
	return (-1);
}
