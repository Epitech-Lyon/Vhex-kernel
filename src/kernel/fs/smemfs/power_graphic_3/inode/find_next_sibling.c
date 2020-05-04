#include <kernel/fs/smemfs.h>
#include <kernel/util/atomic.h>
#include <kernel/devices/earlyterm.h>

void *smemfs_USB3_find_next_sibling(void *inode)
{
	extern struct smemfs_USB3_superblock smemfs_USB3_superblock;
	struct smemfs_USB3_inode *smemfs_USB3_inode;
	struct smemfs_USB3_inode *sibling_inode;

	// Check error
	if (inode == NULL)
		return (NULL);

	// Get smemfs inode
	smemfs_USB3_inode = inode;

	// Get fake root and get next sibling if possible 
	atomic_start();
	sibling_inode = NULL;
	if (inode != smemfs_USB3_superblock.fake_root_inode)
		sibling_inode = smemfs_USB3_inode->sibling;
	atomic_stop();

	// Debug
	//if (sibling_inode != NULL) {
	//	earlyterm_write("Sibling: %s$\n", sibling_inode->name);
	//	DBG_WAIT;
	//}

	// Return next sibling inode
	return (sibling_inode);
}
