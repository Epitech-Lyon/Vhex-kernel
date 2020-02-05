#ifndef __KERNEL_MEMORY_H__
# define __KERNEL_MEMORY_H__

#include <stddef.h>
#include <stdint.h>

#define PM_BLOCK_SIZE	(64)

struct pm_block_cache_s
{
	// Block status
	enum {
		UNUSED,
		USED
	} status;

	// Space informations.
	uint32_t start;
	uint32_t end;

	// Linked list
	struct pm_block_cache_s *next;
};

struct memory_info_s
{
	// Cache informations.
	struct pm_block_cache_s *cache;
	struct pm_block_cache_s *head;

	// RAM informations.
	uint32_t start;
	uint32_t blocks;
};

// Function
extern void *pm_alloc(size_t size);
extern void pm_free(void *ptr);

#endif /*__KERNEL_MEMORY_H__*/
