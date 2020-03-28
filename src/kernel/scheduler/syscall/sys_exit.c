#include <kernel/scheduler.h>
#include <kernel/util/atomic.h>
#include <kernel/devices/earlyterm.h>
#include <kernel/memory.h>
// TODO: remove / move me ?
#include <lib/unistd.h>

void sys_exit(int status)
{
	extern struct process *process_current;

	// Fow now, just wait manual reset...
	//earlyterm_clear();
	//earlyterm_write("PROCESS EXIT SYSCALL !!!!\n");
	//earlyterm_write("Wait manual reset...");
	
	// Start atomic operation
	atomic_start();

	// Generate stat_loc
	// TODO: signal
	process_current->__stat_loc = __W_EXITCODE(status, 0); 

	// Frre'd all allocated space
	pm_pages_free(process_current->memory.stack.user);
	pm_pages_free(process_current->memory.stack.kernel);
	pm_pages_free(process_current->memory.program.start);
	pm_pages_free(process_current->memory.exit.start);

	// Change process state
	process_current->status = PROC_ZOMBIE;

	// Stop atomic operation
	atomic_stop();
	while (1);
}
