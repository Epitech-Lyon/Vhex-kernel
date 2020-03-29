#include <kernel/util/kmem.h>
#include <kernel/util/atomic.h>
#include <kernel/memory.h>

void *kmem_realloc(void *ptr, size_t size)
{
	extern struct memory_info pmemory;
	void *ret;

	// Check malloc
	if (ptr == NULL)
		return (kmem_alloc(size));

	// Check free
	if (size == 0) {
		kmem_free(ptr);
		return (0);
	}

	// Call common realloc part.
	atomic_start();
	ret = pm_heap_realloc(&pmemory.kheap, ptr, size);
	atomic_stop();
	return (ret);
}
