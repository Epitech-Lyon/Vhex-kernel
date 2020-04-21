#include <kernel/scheduler.h>
#include <bits/waitstatus.h>

void sys_exit(int status)
{
	extern struct process *process_current;

	// Terminate the current process
	process_terminate(process_current, __W_EXITCODE(status, 0));

	//TODO: force schedule !!
	while (1);
}
