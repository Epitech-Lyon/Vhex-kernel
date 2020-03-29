#include <kernel/util/kmem.h>
#include <kernel/util/atomic.h>
#include <kernel/memory.h>

void *kmem_alloc(size_t size)
{
	extern struct memory_info pmemory;
	void *ret;

	atomic_start();
	ret = pm_heap_alloc(&pmemory.kheap, size);
	atomic_stop();
	return (ret);
}
