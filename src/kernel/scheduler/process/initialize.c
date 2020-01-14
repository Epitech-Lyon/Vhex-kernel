#include <kernel/process.h>

// Create all internal global
// used to handle process.
struct process_stack process_stack[PROCESS_MAX];

__attribute__((constructor))
void process_constructor(void)
{
	// Set all process to idle state.
	for (int i = 0 ; i < PROCESS_MAX ; i = i + 1)
	{
		process_stack[i].status = PROC_IDLE;
	}
}
