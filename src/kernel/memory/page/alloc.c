#include <kernel/memory.h>
#include <kernel/util/atomic.h>

static struct pm_page *page_alloc(void)
{
	extern struct memory_info pmemory;
	int i;

	i = -1;
	while (++i < pmemory.cache.pages.number)
	{
		// Check if the block are used
		if (pmemory.cache.pages.list[i].status == PAGE_USED)
			continue;

		// Initialize block and return address
		pmemory.cache.pages.list[i].status = PAGE_USED;
		pmemory.cache.pages.list[i].next = NULL;
		return (&pmemory.cache.pages.list[i]);
	}
	return (NULL);
}

void *pm_pages_alloc(int nb_page)
{
	extern struct memory_info pmemory;
	uint16_t page_entry_id;
	struct pm_page **head;
	struct pm_page *new;

	// Check obvious error.
	if (nb_page == 0 || nb_page >= pmemory.cache.pages.number)
		return (NULL);

	// start atomic operations
	atomic_start();

	// Find block entry.
	page_entry_id = 0;
	head = &pmemory.cache.delta;
	while (*head != NULL)
	{
		// Check is it'is the last allocated object.
		if ((*head)->next == NULL)
		{
			// Check potential memory available.
			if ((*head)->id.end + nb_page >= pmemory.cache.pages.number) {
				atomic_stop();
				return (NULL);
			}
			
			// Get cache entry.
			page_entry_id = (*head)->id.end;
			head = &(*head)->next;
			break;
		}

		// Calculate the gap between current allocated object
		// and the next object and see if we can split the area
		if ((*head)->next->id.start - (*head)->id.end >= nb_page)
		{
			page_entry_id = (*head)->id.end;
			head = &(*head)->next;
			break;
		}

		// Get next allocated block.
		head = &(*head)->next;
	}

	// Setup new allocated block
	new = page_alloc();
	if (new == NULL) {
		atomic_stop();
		return (NULL);
	}

	// Initialize new block
	new->id.start = page_entry_id;
	new->id.end = page_entry_id + nb_page;

	// Insert new block.
	new->next = *head;
	*head = new;

	// stop atomic operations
	atomic_stop();

	// Generate physical memory address
	return ((void*)((page_entry_id * PM_PAGE_SIZE) + pmemory.cache.pages.base_addr));
}
