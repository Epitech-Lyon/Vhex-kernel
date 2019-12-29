#include <kernel/memory.h>
#include <lib/display.h>

// Internal data.
struct memory_info_s pmemory;

__attribute__((constructor(101)))
void memory_init(void)
{
	extern uint32_t ram_start;
	uint32_t ram_end;
	uint32_t ram_size;

	// TODO: determine RAM's end.
	ram_end = 0x88080000;
	ram_size = ram_end - (uint32_t)&ram_start;

	// DEBUG
	/*dclear();
	dprint(0, 0, "RAM diagnostic");
	dprint(0, 1, "start = %p", &ram_start);
	dprint(0, 2, "end   = %p", ram_end);
	dprint(0, 3, "size  = %dko", ram_size / 1024);
	dupdate();*/
	//for (int i = 0 ; i < 9000000 ; i = i + 1);

	// Get the number of block available
	// and calculate the real numer of block with
	// the cache.
	// TODO: check if RAM can be used ? (block == 0)
	// @note: try to avoid too long search part.
	pmemory.cache = (void*)&ram_start;
	pmemory.blocks = (ram_size / PM_BLOCK_SIZE) >> 1;
	ram_end = ram_end - (PM_BLOCK_SIZE * pmemory.blocks);
	while ((uint32_t)&pmemory.cache[pmemory.blocks] < ram_end - PM_BLOCK_SIZE)
	{
		pmemory.blocks = pmemory.blocks + 1;
		ram_end = ram_end - PM_BLOCK_SIZE;
	}

	// Get "real" physical memory start
	pmemory.start = ram_end;

	// DEBUG
	/*dclear();
	dprint(0, 0, "Cache diagnostic");
	dprint(0, 1, "Bloks = %d (%do)", pmemory.blocks, PM_BLOCK_SIZE);
	dprint(0, 2, "Start = %p", pmemory.cache);
	dprint(0, 3, "PRAM  = %p", pmemory.start);
	dupdate();
	for (int i = 0 ; i < 9000000 ; i = i + 1);*/

	// Initialize cache
	for (uint32_t i = 0 ; i < pmemory.blocks ; i = i + 1)
	{
		pmemory.cache[i].status = UNUSED;
		pmemory.cache[i].next = NULL;
	}
}
