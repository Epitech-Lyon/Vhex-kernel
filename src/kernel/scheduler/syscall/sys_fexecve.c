#include <kernel/process.h>
#include <kernel/scheduler.h>
#include <kernel/loader.h>
#include <kernel/util/atomic.h>
#include <kernel/devices/earlyterm.h>

pid_t sys_fexecve(const char *pathname)
{
	struct process *proc;
	pid_t child_pid;
	int error;

	// Start atomic operation
	atomic_start();

	// Try create new process
	proc = process_create(pathname);
	if (proc == NULL)
	{
		earlyterm_write("sys_fexecve: alloc error !");
		DBG_WAIT;
		atomic_stop();
		return (-1);
	}

	// Try to load binary into physical memory
	if (loader(proc, pathname) != 0)
	{
		earlyterm_write("sys_fexecve: loader error !");
		DBG_WAIT;
		process_free(proc);
		atomic_stop();
		return (-1);
	}

	// Debug
	earlyterm_write("New proc loaded !");
	DBG_WAIT;

	// Add new process into task queue
	error = sched_add_task(proc);
	if (error != 0)
	{
		earlyterm_write("sys_fexecve: scheduler error !");
		DBG_WAIT;
		process_free(proc);
		atomic_stop();
		return (-1);
	}

	// Get child process PID
	child_pid = process_get_pid(proc);

	// Debug
	earlyterm_write("New proc sched added !");
	earlyterm_write("New proc PID = %#x !", child_pid);
	DBG_WAIT;

	// Stop atomic operations
	atomic_stop();
	return (child_pid);
}
