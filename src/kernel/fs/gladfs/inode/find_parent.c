#include <kernel/fs/gladfs.h>

/* gladfs_find_parent() - Retrun the parent inode of a file (async) */
void *gladfs_find_parent(void *inode)
{
	// Check inode fault
	if (inode == NULL)
		return (NULL);

	// Retrun the parent inode
	return (((struct gladfs_inode_s*)inode)->parent);
}
