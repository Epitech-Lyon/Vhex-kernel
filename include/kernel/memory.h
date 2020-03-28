#ifndef __KERNEL_MEMORY_H__
# define __KERNEL_MEMORY_H__

#include <stddef.h>
#include <stdint.h>

#define PM_PAGE_SIZE	(256)
#define PM_SIZE_TO_PAGES(size)		((size + PM_PAGE_SIZE - 1) / PM_PAGE_SIZE)

// mapping flags
// Protection (unused)
#define PROT_READ	0x1		/* Page can be read.  */
#define PROT_WRITE	0x2		/* Page can be written.  */
#define PROT_EXEC	0x4		/* Page can be executed.  */
#define PROT_NONE	0x0		/* Page can not be accessed.  */

// Mapping flags
#define MAP_SHARED	0x01		/* Share changes.  */
#define MAP_PRIVATE	0x02		/* Changes are private.  */
#define MAP_ANONYMOUS	0x20		/* Don't use a file.  */
#define MAP_ANON	MAP_ANONYMOUS	/* Don't use a file.  */
#define MAP_FILE	0		/* Ignored.  */
#define MAP_GROWSDOWN	0x00100		/* Stack-like segment.  */
#define MAP_DENYWRITE	0x00800		/* ETXTBSY.  */
#define MAP_EXECUTABLE	0x01000		/* Mark it as an executable.  */
#define MAP_LOCKED	0x02000		/* Lock the mapping.  */
#define MAP_NORESERVE	0x04000		/* Don't check for reservations.  */
#define MAP_POPULATE	0x08000		/* Populate (prefault) pagetables.  */
#define MAP_NONBLOCK	0x10000		/* Do not block on IO.  */
#define MAP_STACK	0x20000		/* Allocation is for a stack.  */
#define MAP_HUGETLB	0x40000		/* Create huge page mapping.  */
#define MAP_SYNC	0x80000		/* Perform synchronous page
					   faults for the mapping.  */
#define MAP_FIXED_NOREPLACE 0x100000	/* MAP_FIXED but do not unmap
					   underlying mapping.  */

// Value returned when mmap value
#define MAP_FAILED	((void*) -1)



#define PM_HEAP_BLOCK_MIN	(16)
struct pm_heap_block
{
	uint16_t status;		/* status of the block (1=used,0=free) */
	uint16_t size;			/* size of the block (without block header) */
} __attribute__((packed, aligned(2)));

struct pm_heap_page
{
	void *brk;			/* break (address of the limit area) */
	size_t size;			/* Page size (without header) */
	struct pm_heap_page *next;	/* Next heap page */
	struct pm_heap_block heap;	/* Start of block informations */
} __attribute__((packed, aligned(4)));

// Real physical memory informations
struct pm_page
{
	// page status
	enum {
		PAGE_USED,
		PAGE_UNUSED
	} status;

	// page ID
	struct {
		uint16_t start;
		uint16_t end;
	} id;

	// next page
	struct pm_page *next;
};

struct memory_info
{
	// Cache informations.
	// @note: use delta list algorithm
	// TODO: use AVL algo
	struct {
		struct pm_page *delta;
		struct {
			int number;
			size_t size;
			struct pm_page *list;
			void *base_addr;
		} pages;
	} cache;

	// RAM informations
	struct {
		void *start;
		void *end;
		size_t size;
	} ram;

	// Kernel heap
	struct pm_heap_page *kheap;

	// Shared pages
//	struct {

//	} shared;

};

// Function
extern void *pm_alloc(size_t size);
extern void pm_free(void *ptr);
extern void pm_debug(void);

// Page allocator
extern void *pm_pages_alloc(int nb_pages);
extern void pm_pages_free(void *addr);


#endif /*__KERNEL_MEMORY_H__*/
