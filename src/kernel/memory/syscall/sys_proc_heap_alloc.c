#include <kernel/memory.h>
#include <kernel/syscall.h>
#include <kernel/process.h>
#include <kernel/util/atomic.h>

void *sys_proc_heap_alloc(size_t size)
{
	extern struct process *process_current;
	void *ret;

	// start atomic operations
	atomic_start();

	// Try to alloc new uninitialized area
	ret = pm_heap_alloc(&process_current->memory.heap, size);

	// Stop atomic operation
	atomic_stop();
	return (ret);
}
