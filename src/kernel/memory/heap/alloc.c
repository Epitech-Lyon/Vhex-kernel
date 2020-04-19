#include <kernel/memory.h>
#include <kernel/util/atomic.h>
#include <kernel/devices/earlyterm.h>

static void *new_pages(struct pm_heap_page **page, size_t size)
{
	int nb_page;

	// calculate the number of page
	nb_page = (size + sizeof(struct pm_heap_page) + PM_PAGE_SIZE - 1) / PM_PAGE_SIZE;

	// Create new page
	//earlyterm_write("Try to get %d page(s)...", nb_page);
	//DBG_WAIT;
	*page = pm_pages_alloc(nb_page);
	if (*page == NULL)
		return (NULL);
	//earlyterm_write("OK!\n", nb_page);

	// Initialize new page
	(*page)->next = NULL;
	(*page)->size = (nb_page * PM_PAGE_SIZE) - sizeof(struct pm_heap_page);
	(*page)->brk = ((void*)(*page)) + (nb_page * PM_PAGE_SIZE); 

	// Initialize first block
	(*page)->heap.status = 0;
	(*page)->heap.size = (*page)->size;
	return (pm_block_split(&(*page)->heap, size));
}

static void *pm_heap_page_check(struct pm_heap_page *page, size_t size)
{
	struct pm_heap_block *block;
	size_t rest_size;
	void *ret;

	// Walk into the page and check each block x_x
	block = &page->heap;
	rest_size = page->size;
	while ((void*)&block[1] < page->brk && rest_size > size)
	{
		// Check if the block is used or not
		if (block->status == 0)
		{
			// Try to split the area
			ret = pm_block_split(block, size);
			if (ret != NULL)
				return (ret);
		}

		// Update space rest and get new block
		rest_size = rest_size - (block->size + sizeof(struct pm_heap_block));
		block = (void*)&block[1] + block->size;
	}
	return (NULL);
}

// Kernel malloc wrapper
void *pm_heap_alloc(struct pm_heap_page **page, size_t size)
{
	void *ret;

	// Force 4-align
	size = (size + 3) >> 2 << 2;

	// start atomic operations
	atomic_start();

	// Walk into each pages x_x
	while (*page != NULL)
	{
		// Check "page" size
		if ((*page)->size < size) {
			page = &(*page)->next;
			continue;
		}

		// Try to find free space in the page
		ret = pm_heap_page_check(*page, size);
		if (ret != NULL) {
			atomic_stop();
			return (ret);
		}

		// Get next page
		page = &(*page)->next;
	}

	// Try to allocate new pages
	ret = new_pages(page, size);

	// stop atomic operations
	atomic_stop();
	return (ret);
}
