#include <kernel/memory.h>
#include <kernel/syscall.h>
#include <kernel/process.h>
#include <kernel/util/atomic.h>

void *sys_proc_heap_realloc(void *ptr, size_t size)
{
	extern struct process *process_current;
	void *ret;

	// start atomic operations
	atomic_start();

	// Try to alloc new uninitialized area
	ret = pm_heap_realloc(&process_current->memory.heap, ptr, size);

	// Stop atomic operation
	atomic_stop();
	return (ret);
}
