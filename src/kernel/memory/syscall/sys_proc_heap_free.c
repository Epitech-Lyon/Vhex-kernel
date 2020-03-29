#include <kernel/memory.h>
#include <kernel/syscall.h>
#include <kernel/process.h>
#include <kernel/util/atomic.h>

void sys_proc_heap_free(void *ptr)
{
	extern struct process *process_current;

	// start atomic operations
	atomic_start();

	// Freed allocated area
	pm_heap_free(process_current->memory.heap, ptr);

	// Stop atomic operation
	atomic_stop();
}
