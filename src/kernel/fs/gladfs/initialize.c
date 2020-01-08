#include <kernel/fs/gladfs.h>
#include <kernel/fs/filesystem.h>
#include <kernel/memory.h>

// Kernel FS block
struct file_system_type gladfs_filesystem =
{
	// internal informations
	.fs_name	= "gladfs",
	.fs_flags	= FS_RDWR | FS_NO_DEVICE,

	// FS specific openrations	
	.filesystem_operations = {
		.mount  = &gladfs_mount,
		.umount = &gladfs_umount
	},

	// File operations
	.file_operations = {
		.read  = &gladfs_read,
		.write = &gladfs_write
	},

	// Inode operations
	.inode_operations = {
		// Walk
		.find_next_sibling	= &gladfs_find_next_sibling,
		.find_first_child	= &gladfs_find_first_child,
		.find_parent		= &gladfs_find_parent,

		// Memory
		.create			= &gladfs_creat,
		.mkdir			= &gladfs_mkdir,
		.rmdir			= NULL,
		.rename			= NULL,

		// Informations
		.get_name		= &gladfs_get_name,
		.get_mode		= &gladfs_get_mode
	}
};

// Internla Glados FS information
struct gladfs_superblock_s gladfs_superblock;

// Contructor for the superBlock
void gladfs_initialize(void)
{
	// initilaize ROOT inode
	gladfs_superblock.root_inode = NULL;

	// Initialize device informations
	gladfs_superblock.block_size = PM_BLOCK_SIZE;

	// Iniitlaize superblock openrations
	gladfs_superblock.super_op.alloc_inode = &gladfs_alloc_inode;
	gladfs_superblock.super_op.destroy_inode = &gladfs_destroy_inode;
	gladfs_superblock.super_op.alloc_fragdata = &gladfs_alloc_fragdata;
	gladfs_superblock.super_op.destroy_fragdata = &gladfs_destroy_fragdata;
}
