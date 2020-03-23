#include <kernel/process.h>

// Internal symbols
struct process *proc_table;
uint32_t proc_table_max;

__attribute__((constructor))
void process_constructor(void)
{
	// Initialise dynamic process list
	// TODO:
	// get the scheduler timing and set the maximal
	// number of processes which can be running in the
	// same time.
	proc_table = NULL;
	proc_table_max = PROCESS_MAX;
}
