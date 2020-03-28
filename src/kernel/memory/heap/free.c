#include <kernel/memory.h>
#include <kernel/util/atomic.h>
#include <kernel/devices/earlyterm.h>

static void pm_block_backmerge(struct pm_heap_block **block,
		struct pm_heap_block *parent)
{
	// Check back-merge possibility
	if (parent == NULL || parent->status != 0)
		return;

	// Absorb current block
	parent->size += (*block)->size + sizeof(struct pm_heap_block);

	// Switch current block
	*block = parent;
}

static void pm_block_frontmerge(struct pm_heap_block *block, void *brk)
{
	struct pm_heap_block *block_front;

	// Check front-merge possibility
	block_front = (void*)&block[1] + block->size;
	if ((void*)block_front >= brk || block_front->status != 0)
		return;

	// Absorb front block
	block->size += block_front->size + sizeof(struct pm_heap_block);
}

static int pm_block_free(struct pm_heap_page *page, void *ptr)
{
	struct pm_heap_block *block_parent;
	struct pm_heap_block *block;

	block_parent = NULL;
	block = &page->heap;
	while ((void*)block < page->brk)
	{
		// check block validity
		if ((void*)&block[1] != ptr)
		{
			block_parent = block;
			block = (void*)&block[1] + block->size;
			continue;
		}

		// Try back-merge / front merge
		pm_block_backmerge(&block, block_parent);
		pm_block_frontmerge(block, page->brk);

		// Update block status
		block->status = 0;
		return (0);
	}
	return (-1);
}

void pm_free(void *ptr)
{
	extern struct memory_info pmemory;
	struct pm_heap_page *page;

	// Start atomic operations
	atomic_start();

	// Try to find the page
	page = pmemory.kheap;
	while (page != NULL)
	{
		// If is the page is found
		if (ptr > (void*)page && ptr < page->brk)
		{
			// Check if the block is not found
			if (pm_block_free(page, ptr) != 0)
				break;

			// Stop atomic operations and return
			atomic_stop();
			return;
		}
		// Get the next page
		page = page->next;
	}
	
	// No block found, display error.
	earlyterm_write(
		"pm_free: Warning, you try to free unused"
		" or allocated memory (%p)", ptr
	);
	DBG_WAIT;

	// Stop atomic operations
	atomic_stop();
}
