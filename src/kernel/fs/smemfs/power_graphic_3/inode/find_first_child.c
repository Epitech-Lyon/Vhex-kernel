#include <kernel/fs/smemfs.h>
#include <kernel/util/atomic.h>
#include <kernel/devices/earlyterm.h>

void *smemfs_USB3_find_first_child(void *inode)
{
	extern struct smemfs_USB3_superblock smemfs_USB3_superblock;
	struct smemfs_USB3_inode *smemfs_USB3_inode;
	struct smemfs_USB3_inode *child_inode;

	// Check error
	if (inode == NULL)
		return (NULL);

	// Get smemfs inode
	smemfs_USB3_inode = inode;

	// Check fake root and get child if possible
	atomic_start();
	child_inode = smemfs_USB3_superblock.root_inode;
	if (inode != smemfs_USB3_superblock.fake_root_inode)
		child_inode = smemfs_USB3_inode->child;
	atomic_stop();

	// Debug
	//earlyterm_write("First child %s$\n", child_inode->name);
	//DBG_WAIT;

	// Return child inode
	return (child_inode);
}
