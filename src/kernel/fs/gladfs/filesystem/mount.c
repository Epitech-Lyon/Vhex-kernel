#include <kernel/fs/gladfs.h>
#include <kernel/atomic.h>
#include <kernel/util.h>

/* gladfs_mount() - GladFS mount primitive (sync) */
void *gladfs_mount(void)
{
	extern struct gladfs_superblock_s gladfs_superblock;
	void *root_inode;

	// Start atomic operation
	atomic_start();

	// If the root node does not exist...
	if (gladfs_superblock.root_inode == NULL)
	{
		// ...Try to create root inode.
		gladfs_superblock.root_inode = gladfs_superblock.super_op.alloc_inode("/", GLADFS_INODE_TYPE_ROOT);
		if (gladfs_superblock.root_inode == NULL)
		{
			kvram_clear();
			kvram_print(0, 0, "GladFS: ROOT inode alloc error !");
			kvram_display();
			DBG_WAIT;
		}
	}

	// Get root inode
	// @note: In theroy, the super block is
	// stored on the device so, to ensure the
	// synchronous primitives, we get the root
	// inode here to restore interrupt before
	// leave.
	root_inode = gladfs_superblock.root_inode;

	// Stop atomic operations
	atomic_stop();

	// Return root inode 
	return (root_inode);
}
