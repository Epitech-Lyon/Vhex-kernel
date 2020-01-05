#include <kernel/fs/gladfs.h>

void *gladfs_find_parent(void *inode)
{
	if (inode == NULL)
		return (NULL);
	return (((struct gladfs_inode_s*)inode)->parent);
}
