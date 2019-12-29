#include <kernel/memory.h>

static struct pm_block_cache_s *block_alloc(void)
{
	extern struct memory_info_s pmemory;
	uint32_t i;

	i = -1;
	while (++i < pmemory.blocks)
	{
		// Check if the block are used
		if (pmemory.cache[i].status == USED)
			continue;

		// Initialize block and return address
		pmemory.cache[i].status = USED;
		pmemory.cache[i].next = NULL;
		return (&pmemory.cache[i]);
	}
	return (NULL);
}

void *pm_alloc(size_t size)
{
	extern struct memory_info_s pmemory;
	struct pm_block_cache_s **head;
	struct pm_block_cache_s *block;
	uint32_t block_entry;
	uint32_t nb_blocks;

	// Check obvious error.
	if (size == 0)
		return (NULL);

	// Get the number of blocks we need.
	nb_blocks = (size + PM_BLOCK_SIZE - 1) / PM_BLOCK_SIZE;

	// Find block entry.
	block_entry = 0;
	head = &pmemory.head;
	while (*head != NULL)
	{
		// Check is it is the last allocated
		// object.
		if ((*head)->next == NULL)
		{
			// Check memory space.
			if ((*head)->end + 1 + nb_blocks >= pmemory.blocks)
				return (NULL);
			
			// Get cache entry.
			block_entry = (*head)->end + 1;
			head = &(*head)->next;
			break;
		}

		// Calculate the gap between current
		// allocated object and the next object.
		if (((*head)->next->start) - ((*head)->end + 1) >= nb_blocks)
		{
			block_entry = (*head)->end + 1;
			head = &(*head)->next;
			break;
		}

		// Get next allocated block.
		head = &(*head)->next;
	}

	// Setup new allocated block
	block = block_alloc();
	if (block == NULL)
		return (NULL);

	// Initialize new block
	block->start = block_entry;
	block->end = block_entry + nb_blocks - 1;

	// Insert new block.
	block->next = *head;
	*head = block;

	// Generate physical memory address
	return ((void*)((block_entry * PM_BLOCK_SIZE) + pmemory.start));
}
