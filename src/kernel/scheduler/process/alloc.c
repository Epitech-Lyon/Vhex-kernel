#include <kernel/process.h>
#include <kernel/devices/earlyterm.h>
#include <kernel/util/atomic.h>
#include <kernel/memory.h>

// TODO: return EAGAIN !!
// TODO: return ENOMEM !!
struct process *process_alloc(void)
{
	extern struct process *proc_table;
	extern uint32_t proc_table_max;
	uint32_t proc_table_idx;
	struct process **proc;

	// Start atomic operation
	atomic_start();

	// Try to find free slot
	proc = &proc_table;
	proc_table_idx = -1;
	while (++proc_table_idx < proc_table_max && *proc != NULL)
	{
		// Check free slot
		if ((*proc)->status != PROC_DEAD) {
			proc = &(*proc)->next;
			continue;
		}
	
		// Indicate init phase, stop atomic
		// operations and return the process
		(*proc)->status = PROC_INIT;
		(*proc)->pgid = 0;
		atomic_stop();
		return (*proc);
	}

	// Check error
	if (proc_table_idx >= proc_table_max) {
		earlyterm_write("proc_alloc: EAGAIN !\n");
		atomic_stop();
		return (NULL);
	}

	// Alloc new process manually
	*proc = (struct process *)pm_alloc(sizeof(struct process));
	if (*proc == NULL) {
		earlyterm_write("proc_alloc: ENOMEM !\n");
		atomic_stop();
		return (NULL);
	}

	// Init and return the now process
	(*proc)->status = PROC_INIT;
	(*proc)->pid = proc_table_idx + 1;
	(*proc)->pgid = 0;
	(*proc)->next = NULL;

	// Stop atomic operations
	atomic_stop();
	return (*proc);
}
