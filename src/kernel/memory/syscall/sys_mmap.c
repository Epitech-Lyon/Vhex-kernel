#include <kernel/memory.h>
#include <kernel/syscall.h>
#include <kernel/devices/earlyterm.h>
#include <kernel/fs/vfs.h>

//TODO: udpate me !!
//TODO: handle flags !!
void *sys_mmap(void *addr, size_t length,
		int prot, int flags, int fd, off_t offset)
{
	extern struct process *process_current;

	// Check process error
	if (process_current == NULL)
		return (MAP_FAILED);

	// @note: we can not handle user-request for
	// the address, because we can not use the
	// virtual memory
	if (addr != NULL)
		return (MAP_FAILED);

	// @note: we can not handle protactions for
	// the pages because we can not use the
	// virtual memory
	(void)prot;

	//TODO: handle flags !!!
	(void)flags;
	
	// Calculated the number of pages needed
	int nb_pages = (length + PM_PAGE_SIZE - 1) / PM_PAGE_SIZE;

	// Try to get all pages needed
	//void *area = pm_get_pages(nb_pages);
	//if (area == NULL)
		return (MAP_FAILED);

	// Check if we must copy the file content into
	// new area.
	//if (fd >= 0)
	//	sys_pread(fd, area, length, offset);
	//return (area);
}
