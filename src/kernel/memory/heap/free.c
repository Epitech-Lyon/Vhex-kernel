#include <kernel/memory.h>
#include <kernel/util/atomic.h>
#include <kernel/devices/earlyterm.h>

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

void pm_heap_free(struct pm_heap_page *page, void *ptr)
{
	// Start atomic operations
	atomic_start();

	// Try to find the page
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
