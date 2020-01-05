/*#include <kernel/fs/vfs.h>

int sys_open(const char *parthname, int flags, ...)
{
	// TODO: handle flag O_CREAT and mode
	(void)flags;

	// Try to find inode
	inode = vfs_open(pathname, flags);
	if (inode == NULL)
		return (-1);

	// TODO: find process file slot !
	// TODO: initialise file informations !
	return (-1);
}*/
