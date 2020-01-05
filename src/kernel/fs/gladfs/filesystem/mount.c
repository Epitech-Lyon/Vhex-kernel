#include <kernel/fs/gladfs.h>
#include <kernel/util.h>

void *gladfs_mount(void)
{
	extern struct gladfs_superblock_s gladfs_superblock;

	// If already mounted, return the root inode
	if (gladfs_superblock.root_inode != NULL)
		return (gladfs_superblock.root_inode);

	// Try to create root inode.
	gladfs_superblock.root_inode = gladfs_superblock.super_op.alloc_inode("/", GLADFS_INODE_TYPE_ROOT);
	if (gladfs_superblock.root_inode == NULL)
	{
		kvram_clear();
		kvram_print(0, 0, "GladFS: ROOT inode alloc error !");
		kvram_display();
		DBG_WAIT;
		return (NULL);
	}

	// Return root inode 
	return (gladfs_superblock.root_inode);
}
