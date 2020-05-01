#include <kernel/process.h>
#include <kernel/scheduler.h>
#include <kernel/devices/earlyterm.h>
#include <kernel/util/atomic.h>
#include <kernel/fs/vfs.h>
#include <kernel/memory.h>

static void first_proc_term(int __stat_lock)
{
	earlyterm_write("Kernel PANIK !!\n");
	earlyterm_write("First process terminate [%d]\n", __stat_lock);
	
	// TODO: restore Casio context / environnement
	earlyterm_write("Wait manual reset...");
	while (1);
}

void process_terminate(struct process *proc, int __stat_lock)
{
	struct pm_heap_page **page;
	void *tmp;

	// Check first process termination
	if (proc->parent == NULL)
		first_proc_term(__stat_lock);

	// Start atomic operation
	atomic_start();

	// Close all opened file
	for (int i = 0 ; i < PROCESS_NB_OPEN_FILE ; ++i)
	{
		if (proc->opfile[i].status == PROCESS_FILE_SLOT_USED)
			vfs_close(&proc->opfile[i].file);
	}

	// Free'd all allocated space
	pm_pages_free(proc->memory.stack.user);
	pm_pages_free(proc->memory.stack.kernel);
	pm_pages_free(proc->memory.program.start);
	page = &proc->memory.heap;
	while (*page != NULL) {
		tmp = (*page)->next;
		pm_pages_free(*page);
		*page = tmp;
	}

	// Secure error
	proc->memory.stack.user = NULL;
	proc->memory.stack.kernel = NULL;
	proc->memory.program.start = NULL;

	// Change scheduler task state
	proc->sched_task->status = SCHED_TASK_ZOMBIE;

	// Generate stat_loc
	proc->__stat_loc = __stat_lock; 

	// Stop atomic operation
	atomic_stop();
}
