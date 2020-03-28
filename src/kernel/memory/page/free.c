#include <kernel/memory.h>
#include <kernel/util/atomic.h>
#include <kernel/devices/earlyterm.h>

void pm_pages_free(void *ptr)
{
	extern struct memory_info pmemory;
	uint16_t page_entry_id;
	struct pm_page **head;
	uint32_t sptr;

	// Save address for error message.
	sptr = (uint32_t)ptr;

	// Get the "real" physical space.
	ptr = (void*)(ptr - pmemory.cache.pages.base_addr);

	// Check misaligned pointer.
	if (((uint32_t)ptr % PM_PAGE_SIZE) != 0)
	{
		earlyterm_write(
			"pm_free: Warning, you try to free misaligned"
			"pointer address (%p)\n", sptr
		);
		DBG_WAIT;
		return;
	}

	// Get block entry.
	page_entry_id = (uint32_t)ptr / PM_PAGE_SIZE;

	// Start atomic operations
	atomic_start();

	// Walk into "head" cache and try to find 
	// the allocated block.
	head = &pmemory.cache.delta;
	while (*head != NULL)
	{
		// Check the allocated block.
		if ((*head)->id.start != page_entry_id)
		{
			head = &(*head)->next;
			continue;
		}

		// Free the block and return
		(*head)->status = PAGE_UNUSED;
		*head = (*head)->next;
		atomic_stop();
		return;
	}

	// No block found, display error.
	earlyterm_write(
		"pm_free: Warning, you try to free unused"
		"allocated memory (%p)", sptr
	);
	DBG_WAIT;

	// Stop atomic operations
	atomic_stop();
}
