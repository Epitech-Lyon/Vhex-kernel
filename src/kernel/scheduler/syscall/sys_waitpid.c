#include <kernel/process.h>
#include <kernel/scheduler.h>
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
			if ((*proc)->sched_task->status == SCHED_TASK_ZOMBIE &&
				((pid < -1 && (*proc)->pgid == -pid)
				 || pid == -1
				 || (pid == 0 && (*proc)->pgid == process_current->pgid)
				 || (pid > 0 && (*proc)->pid == pid)))
			{
				// Change the child process state
				// @note: internal address used to determine if
				// the process is alive or not
				(*proc)->parent = (void*)0xdeadbeef;

				// Get the child process PID and stat_loc
				ret = (*proc)->pid;
				*wstatus = (*proc)->__stat_loc;

				// Remove process form the child list
				*proc = (*proc)->sibling;

				// Stop atomic operation and return
				atomic_stop();
				return (ret);
			}

			// Get next child process
			proc = &(*proc)->sibling;
		}

		// Force process to sleep
		// TODO:
		// * Force schedule !!
		// * change the niciess of the process !!
		// * wait SIGCHLD !!!!
		//
		// Stop atomic operation and wait
		atomic_stop();
		__asm__ volatile ("sleep; nop");
	}
	// Error
	return (-1);
}
