#include <kernel/process.h>
#include <kernel/scheduler.h>
#include <kernel/loader.h>
#include <kernel/util/atomic.h>
#include <kernel/devices/earlyterm.h>
#include <lib/string.h>

static void proc_dump_shared(struct process *child, struct process *parent)
{
	// Dump all opened file
	for (int i = 0 ; i < PROCESS_NB_OPEN_FILE ; ++i)
	{
		memcpy(&child->opfile[i].file, &parent->opfile[i].file, sizeof(FILE));
		child->opfile[i].status = parent->opfile[i].status;
	}

	// Dump specific
	memcpy(&child->tty, &parent->tty, sizeof(FILE));
}

pid_t sys_fexecve(const char *pathname)
{
	extern struct process *process_current;
	struct process *proc;
	pid_t child_pid;

	// Start atomic operation
	atomic_start();

	// Try create new process
	proc = process_create(pathname);
	if (proc == NULL)
	{
		earlyterm_write("sys_fexecve: process_create error !\n");
		DBG_WAIT;
		atomic_stop();
		return (-1);
	}

	// Dump parent process shared informations
	if (process_current != NULL)
		proc_dump_shared(proc, process_current);

	// Try to load binary into physical memory
	if (loader(proc, pathname) != 0)
	{
		earlyterm_write("sys_fexecve: loader error !");
		DBG_WAIT;
		process_free(proc);
		atomic_stop();
		return (-1);
	}

	// Add new process into task queue
	if (sched_add_task(proc))
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
