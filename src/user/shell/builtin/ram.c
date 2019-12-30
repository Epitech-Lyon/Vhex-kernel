/*#include "builtin.h"
#include <kernel/memory.h>
#include <lib/display.h>

VHEX_BUILTIN(ram)
{
	extern struct memory_info_s pmemory;
	extern uint32_t ram_start;
	uint32_t ram_end;
	uint32_t ram_size;

	// TODO: determine RAM's end.
	ram_end = 0x88080000;
	ram_size = ram_end - (uint32_t)&ram_start;

	//dprint(0, 0, "RAM diagnostic");
	//dprint(0, 1, "start = %p", &ram_start);
	//dprint(0, 2, "end   = %p", 0x88080000);
	//dprint(0, 3, "size  = %dko", ram_size / 1024);

	//dprint(0, 5, "Cache diagnostic");
	//dprint(0, 0, "Bloks = %d (%do)", pmemory.blocks, PM_BLOCK_SIZE);
	//dprint(0, 1, "Start = %p", pmemory.cache);
	//dprint(0, 2, "PRAM  = %p", pmemory.start);
	//dupdate();

	//TODO: GetKey
	//for (int i = 0 ; i < 9000000 ; i = i + 1);

	// Try to alloc.
	void *test0 = pm_alloc(129);
	void *test1 = pm_alloc(1024);
	void *test2 = pm_alloc(129);

	// Display address
	dclear();
	dprint(0, 0, "test0 = %p (129o)", test0);
	dprint(0, 1, "test1 = %p (1024o)", test1);
	dprint(0, 2, "test2 = %p (129o)", test2);

	// Try to free allocated space.
	pm_free(test1);

	// Try to alloc again.
	test1 = pm_alloc(64);

	// Display RAM abstract cache
	int i = 0;
	struct pm_block_cache_s *head = pmemory.head;
	while (head != NULL && i < 7)
	{
		dprint(0, 3 + i, "s:%d-e:%d-n:%p", head->start, head->end, head->next);
		head = head->next;
		i = i + 1;
	}
	dupdate();
	for (int i = 0 ; i < 9000000 ; i = i + 1);

	return (0);
}*/
