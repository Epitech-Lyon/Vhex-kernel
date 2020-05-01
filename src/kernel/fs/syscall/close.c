#include <kernel/fs/vfs.h>
#include <kernel/process.h>

int sys_close(int fd)
{
	extern struct process *process_current;

	// Check fd validity
	if (fd < 0 || fd >= PROCESS_NB_OPEN_FILE)
		return (-1);

	// Call VFS close primitive
	return (vfs_close(&process_current->opfile[fd].file));
}
