#include <kernel/fs/gladfs.h>
#include <kernel/fs/stat.h>

/* gladfs_find_first_child() - Return the first child of folder inode (async) */
void *gladfs_find_first_child(void *inode)
{
	// Check inode fault
	if (inode == NULL)
		return (NULL);

	// Check inode validity
	if ((((struct gladfs_inode_s*)inode)->mode & __S_IFDIR) == 0)
		return (NULL);

	// Return the first child of the directory.
	return (((struct gladfs_inode_s*)inode)->children);
}
