#include <kernel/process.h>
#include <kernel/scheduler.h>
#include <kernel/loader.h>
#include <kernel/util/atomic.h>
#include <kernel/util/debug.h>

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
		kvram_clear();
		printk(0, 0, "sys_fexecve: alloc error !");
		kvram_display();
		DBG_WAIT;
		atomic_stop();
		return (-1);
	}

	// Try to load binary into physical memory
	proc->context.spc = (uint32_t)loader(pathname, proc);
	if (proc->context.spc == 0x00000000)
	{
		kvram_clear();
		printk(0, 0, "sys_fexecve: loader error !");
		kvram_display();
		DBG_WAIT;
		process_free(proc);
		atomic_stop();
		return (-1);
	}

	// Debug
	kvram_clear();
	printk(0, 0, "New proc loaded !");
	kvram_display();
	DBG_WAIT;

	// Add new process into task queue
	error = sched_add_task(proc);
	if (error != 0)
	{
		kvram_clear();
		printk(0, 0, "sys_fexecve: scheduler error !");
		kvram_display();
		DBG_WAIT;
		process_free(proc);
		atomic_stop();
		return (-1);
	}

	// Debug
	printk(0, 1, "New proc sched added !");
	kvram_display();
	DBG_WAIT;

	// Get child process PID
	child_pid = process_get_pid(proc);

	// Debug
	printk(0, 2, "New proc PID = %#x !", child_pid);
	kvram_display();
	DBG_WAIT;

	// Stop atomic operations
	atomic_stop();
	return (child_pid);
}
