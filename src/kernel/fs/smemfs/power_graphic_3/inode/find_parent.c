#include <kernel/fs/smemfs.h>
#include <kernel/util/atomic.h>

void *smemfs_USB3_find_parent(void *inode)
{
	extern struct smemfs_USB3_superblock smemfs_USB3_superblock;
	struct smemfs_USB3_inode *smemfs_USB3_inode;
	void *parent_inode;

	// Check error
	if (inode == NULL)
		return (NULL);

	// Get smemfs inode
	smemfs_USB3_inode = inode;

	// Get fake root and get parent inode if possible
	atomic_start();
	parent_inode = NULL;
	if (inode != smemfs_USB3_superblock.fake_root_inode)
		parent_inode = smemfs_USB3_inode->parent;
	atomic_stop();

	// Return parent inode
	return (parent_inode);
}
