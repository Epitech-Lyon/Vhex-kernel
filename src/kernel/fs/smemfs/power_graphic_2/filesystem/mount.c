#include <kernel/fs/smemfs.h>
#include <kernel/util/atomic.h>

/* casio_smem_mount() - Mount the file system (sync) */
void *smemfs_USB2_mount(void)
{
	extern struct smemfs_superblock_s smemfs_superblock;
	void *root_inode;

	// Start atomic operation
	atomic_start();

	// Get root inode
	root_inode = smemfs_superblock.sector_table;

	// Stop atomic operation
	atomic_stop();

	// Return the sector table to simulate the root inode.
	return (root_inode);
}
