#include <kernel/fs/gladfs.h>

/* gladfs_find_next_sibling() - Find the next file from the same parent (async) */
void *gladfs_find_next_sibling(void *inode)
{
	// Check inode fault
	if (inode == NULL)
		return (NULL);

	// Return the next inode
	return (((struct gladfs_inode_s*)inode)->next);
}
