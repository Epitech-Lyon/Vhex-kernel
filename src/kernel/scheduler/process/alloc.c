#include <kernel/process.h>
#include <kernel/devices/earlyterm.h>
#include <kernel/util/atomic.h>
#include <kernel/util/kmem.h>

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
		// @note: we use 0xdeadbeef to mark free process
		if ((*proc)->parent != (void*)0xdeadbeef) {
			proc = &(*proc)->next;
			continue;
		}
	
		// Indicate init phase, stop atomic
		// operations and return the process
		(*proc)->parent = NULL;
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

	// If no process is free, alloc new process manually
	*proc = (struct process *)kmem_alloc(sizeof(struct process));
	if (*proc == NULL) {
		earlyterm_write("proc_alloc: ENOMEM !\n");
		atomic_stop();
		return (NULL);
	}

	// Init and return the now process
	(*proc)->parent = NULL;
	(*proc)->pid = proc_table_idx + 1;
	(*proc)->pgid = 0;
	(*proc)->next = NULL;

	// Stop atomic operations
	atomic_stop();
	return (*proc);
}
