#include <kernel/util/kmem.h>
#include <kernel/util/atomic.h>
#include <kernel/memory.h>

void kmem_free(void *ptr)
{
	extern struct memory_info pmemory;

	atomic_start();
	pm_heap_free(pmemory.kheap, ptr);
	atomic_stop();
}
