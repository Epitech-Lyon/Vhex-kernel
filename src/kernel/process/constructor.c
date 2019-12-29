#include <kernel/process.h>

// Create all internal global
// used to handle process.
struct process_stack_s process_stack[PROCESS_MAX];
process_t *process_current;

__attribute__((constructor))
void process_constructor(void)
{
	// Set all process to idle state.
	for (int i = 0 ; i < PROCESS_MAX ; i = i + 1)
	{
		process_stack[i].status = PROC_IDLE;
	}

	// No process is currently running.
	process_current = NULL;
}
