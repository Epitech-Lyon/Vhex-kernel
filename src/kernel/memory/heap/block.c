#include <kernel/memory.h>

void *pm_block_split(struct pm_heap_block *block, size_t size)
{
	int rest_size;
	void *ret;
	
	// Check size
	if (block->size < size)
		return (NULL);

	// Change current block status
	block->status = 1;

	// Check if we can not split
	rest_size = block->size - (size + sizeof(struct pm_heap_block));
	if (rest_size < PM_HEAP_BLOCK_MIN)
		return (&block[1]);

	// Update current block size and get the
	// returned value
	block->size = size;
	ret = &block[1];

	// Initialize seconde block
	block = (void *)&block[1] + block->size;
	block->status = 0;
	block->size = rest_size;
	return (ret);
}

void pm_block_backmerge(struct pm_heap_block **block, struct pm_heap_block *parent)
{
	// Check back-merge possibility
	if (parent == NULL || parent->status != 0)
		return;

	// Absorb current block
	parent->size += (*block)->size + sizeof(struct pm_heap_block);

	// Switch current block
	*block = parent;
}

void pm_block_frontmerge(struct pm_heap_block *block, void *brk)
{
	struct pm_heap_block *block_front;

	// Check front-merge possibility
	block_front = (void*)&block[1] + block->size;
	if ((void*)block_front >= brk || block_front->status != 0)
		return;

	// Absorb front block
	block->size += block_front->size + sizeof(struct pm_heap_block);
}
