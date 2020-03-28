#include <kernel/memory.h>
#include <kernel/devices/earlyterm.h>

// Internal data.
struct memory_info pmemory;

__attribute__((constructor(101)))
void memory_init(void)
{
	extern uint32_t ram_start;
	extern uint32_t srom;

	// Initialize RAM informations
	// TODO determine RAM's size
	pmemory.ram.end = (void *)0x88080000;
	pmemory.ram.start = (void *)(((uintptr_t)&ram_start + 3) >> 2 << 2);
	pmemory.ram.size = pmemory.ram.end - pmemory.ram.start;

	// Generate cache part
	pmemory.cache.delta = NULL;
	pmemory.cache.pages.number = 0;
	pmemory.cache.pages.size = PM_PAGE_SIZE;
	pmemory.cache.pages.list = pmemory.ram.start;
	pmemory.cache.pages.base_addr = pmemory.ram.end;
	while ((void *)&pmemory.cache.pages.list[pmemory.cache.pages.number] <
				pmemory.cache.pages.base_addr - PM_PAGE_SIZE)
	{
		pmemory.cache.pages.base_addr -= PM_PAGE_SIZE;
		pmemory.cache.pages.number += 1;
	}

	// Initialize kernel heap
	pmemory.kheap = NULL;

	// Debug
/*	earlyterm_write("kernel    = %do -> %dko\n", &srom, (uint32_t)&srom / 1024);
	earlyterm_write("ram start = %p\n", pmemory.ram.start);
	earlyterm_write("ram end   = %p\n", pmemory.ram.end);
	earlyterm_write("ram size  = %do -> %dko\n", pmemory.ram.size, pmemory.ram.size / 1024);
	earlyterm_write("page size = %do -> %#xo\n", pmemory.cache.pages.size, pmemory.cache.pages.size);
	earlyterm_write("page num  = %d\n", pmemory.cache.pages.number);
	earlyterm_write("real RAM  = %d -> %dko\n", pmemory.cache.pages.number * PM_PAGE_SIZE, (pmemory.cache.pages.number * PM_PAGE_SIZE) / 1024);
*/
	// Initialize cache page list
	for (int i = 0 ; i < pmemory.cache.pages.number ; i = i + 1)
	{
		pmemory.cache.pages.list[i].status = PAGE_UNUSED;
		pmemory.cache.pages.list[i].next = NULL;
	}

	// TEST part
/*	void *test0 = pm_alloc(16);
	void *test1 = pm_alloc(128);
	void *test2 = pm_alloc(80);
	earlyterm_write("test0 = %p\n", test0);
	earlyterm_write("test1 = %p\n", test1);
	earlyterm_write("test2 = %p\n", test2);
	pm_free(test2);
	pm_free(test0);
	pm_heap_debug();
	DBG_WAIT;
	pm_free(test1);
	pm_debug();
	pm_free((void*)0xa0000000);
	DBG_WAIT;
	while (1);*/
}
