#include <kernel/util/kmem.h>
#include <kernel/util/atomic.h>
#include <kernel/memory.h>

void kmem_debug(void)
{
	extern struct memory_info pmemory;

	atomic_start();
	pm_heap_debug(pmemory.kheap);
	atomic_stop();
}
