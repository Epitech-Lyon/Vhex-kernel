#include <kernel/fs/vfs.h>
#include <kernel/process.h>

ssize_t sys_read(int fd, void *buf, size_t count)
{
	extern process_t *process_current;

	// Check fd
	if (fd < 0 || fd >= PROCESS_NB_OPEN_FILE)
		return (-1);

	// call VFS read primitive
	return (vfs_read(&process_current->opfile[fd].file, buf, count));
}
