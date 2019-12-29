#include <kernel/memory.h>
#include <kernel/devices/tty.h>

void pm_free(void *ptr)
{
	extern struct memory_info_s pmemory;
	struct pm_block_cache_s **head;
	uint32_t block_entry;
	uint32_t sptr;

	// Save address for error message.
	sptr = (uint32_t)ptr;

	// Get the "real" physical space.
	ptr = (void*)(ptr - pmemory.start);

	// Check misaligned pointer.
	if (((uint32_t)ptr % PM_BLOCK_SIZE) != 0)
	{
		tty_write(
			"pm_free: Warning, you try to free misaligned"
			"pointer address (%p)\n", sptr
		);
		return;
	}

	// Get block entry.
	block_entry = (uint32_t)ptr / PM_BLOCK_SIZE;

	// Walk into "head" cache and try to find 
	// the allocated block.
	head = &pmemory.head;
	while (*head != NULL)
	{
		// Check the allocated block.
		if ((*head)->start != block_entry)
		{
			head = &(*head)->next;
			continue;
		}

		// Free the block and return
		(*head)->status = UNUSED;
		*head = (*head)->next;
		return;
	}

	// No block found, display error.
	tty_write(
		"pm_free: Warning, you try to free unused"
		"allocated memory (%p)", sptr
	);
}
