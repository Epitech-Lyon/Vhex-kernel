#include <kernel/scheduler.h>
#include <kernel/util/atomic.h>
#include <kernel/devices/earlyterm.h>
#include <kernel/fs/vfs.h>
#include <kernel/memory.h>
#include <sys/wait.h>

void sys_exit(int status)
{
	extern struct process *process_current;
	struct pm_heap_page *page;
	void *tmp;

	// Start atomic operation
	atomic_start();

	// Close all opened file
	for (int i = 0 ; i < PROCESS_NB_OPEN_FILE ; ++i)
	{
		if (process_current->opfile[i].status == PROCESS_FILE_SLOT_USED)
			vfs_close(&process_current->opfile[i].file);
	}

	// Frre'd all allocated space
	pm_pages_free(process_current->memory.stack.user);
	pm_pages_free(process_current->memory.stack.kernel);
	pm_pages_free(process_current->memory.program.start);
	pm_pages_free(process_current->memory.exit.start);
	page = process_current->memory.heap;
	while (page != NULL) {
		tmp = page->next;
		pm_pages_free(page);
		page = tmp;
	}

	// Change scheduler task state
	process_current->sched_task->status = SCHED_TASK_ZOMBIE;

	// Generate stat_loc
	// TODO: signal !!!
	process_current->__stat_loc = __W_EXITCODE(status, 0); 

	// Stop atomic operation
	atomic_stop();
	while (1);
}
