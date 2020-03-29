#include <kernel/memory.h>
#include <kernel/util/atomic.h>
#include <kernel/devices/earlyterm.h>
#include <string.h>

// Internal function
extern void *pm_block_split(struct pm_heap_block *block, size_t size);
extern void pm_block_backmerge(struct pm_heap_block **block, struct pm_heap_block *parent);
extern void pm_block_frontmerge(struct pm_heap_block *block, void *brk);

static int pm_block_update(struct pm_heap_block *block, size_t size, void *brk)
{
	struct pm_heap_block *block_front;

	// Check front block validity
	block_front = (void*)&block[1] + block->size;
	if ((void*)block_front >= brk || block_front->status != 0)
		return (-1);

	// Check size
	if (block_front->size + sizeof(struct pm_heap_block) + block->size < size)
		return (-1);

	// merge front block and try to split the block
	block->size += block_front->size + sizeof(struct pm_heap_block);
	pm_block_split(block, size);
	return (0);
}

static void *pm_block_realloc(struct pm_heap_page **page, void *ptr, size_t size)
{
	struct pm_heap_block *block_parent;
	struct pm_heap_block *block;
	struct pm_heap_block *new;

	block_parent = NULL;
	block = &(*page)->heap;
	while ((void*)block < (*page)->brk)
	{
		// check block validity
		if ((void*)&block[1] != ptr)
		{
			block_parent = block;
			block = (void*)&block[1] + block->size;
			continue;
		}

		// Check useless action
		if (block->size >= size || pm_block_update(block, size, (*page)->brk) == 0)
			return (&block[1]);

		// Manually alloc new node
		new = pm_heap_alloc(page, size);
		if (new == NULL)
			return (&block[1]);

		// Copie old place
		memcpy(new, &block[1], block->size);

		// Try back-merge / front merge current block
		pm_block_backmerge(&block, block_parent);
		pm_block_frontmerge(block, (*page)->brk);

		// Update block status
		block->status = 0;
		return (new);
	}
	return (NULL);
}

void *pm_heap_realloc(struct pm_heap_page **page, void *ptr, size_t size)
{
	void *ret;

	// Force 4-align
	size = (size + 3) >> 2 << 2;

	// Start atomic operations
	atomic_start();

	// Try to find the page
	while (page != NULL)
	{
		// If is the page is found
		if (ptr > (void*)page && ptr < (*page)->brk)
		{
			// Check if the block is not found
			ret = pm_block_realloc(page, ptr, size);

			// Stop atomic operations and return area
			atomic_stop();
			return (ret);
		}
		// Get the next page
		page = &(*page)->next;
	}
	
	// No block found, display error.
	earlyterm_write(
		"pm_free: Warning, you try to realloc"
		"unused or allocated memory (%p)", ptr
	);
	DBG_WAIT;

	// Stop atomic operations
	atomic_stop();
	return (NULL);
}
