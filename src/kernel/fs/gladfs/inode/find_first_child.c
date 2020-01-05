#include <kernel/fs/gladfs.h>

void *gladfs_find_first_child(void *inode)
{
	if (inode == NULL)
		return (NULL);
	return (((struct gladfs_inode_s*)inode)->children);
}
