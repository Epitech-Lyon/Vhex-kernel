#include <kernel/fs/smemfs.h>
#include <kernel/fs/filesystem.h>
#include <kernel/devices/earlyterm.h>
#include <kernel/util/atomic.h>

// Casio SMEM FileSystem primitives
// @note: We use USB PowerGraphic 2 primitives
struct file_system_type smemfs_filesystem =
{
	// internal informations
	.fs_name	= "smemfs",
	.fs_flags	= FS_RDONLY,

	// FS specific openrations	
	.filesystem_operations = {
		.mount  = &smemfs_USB2_mount,
		.umount = &smemfs_USB2_umount
	},

	// File operations
	.file_operations = {
		.read  = &smemfs_USB2_read,
		.write = NULL,
		.ioctl = NULL
	},

	// Inode operations
	.inode_operations = {
		// Walk
		.find_next_sibling	= &smemfs_USB2_find_next_sibling,
		.find_first_child	= &smemfs_USB2_find_first_child,
		.find_parent		= &smemfs_USB2_find_parent,

		// Memory
		.create			= NULL,
		.mkdir			= NULL,
		.rmdir			= NULL,
		.rename			= NULL,

		// Informations
		.get_name		= &smemfs_USB2_get_name,
		.get_mode		= &smemfs_USB2_get_mode
	}
};

// Internal superblock informations
struct smemfs_USB2_superblock smemfs_USB2_superblock;
struct smemfs_USB3_superblock smemfs_USB3_superblock;


// switch USB Power Graphic II driver to
// USB Power Graphic III (Casio syscall wrapper)
static void use_dump_smem_driver(void)
{
	smemfs_USB3_superblock.root_inode = NULL;
	smemfs_filesystem.filesystem_operations.mount = &smemfs_USB3_mount;
	smemfs_filesystem.filesystem_operations.umount = NULL;
	smemfs_filesystem.file_operations.read = &smemfs_USB3_read;
	smemfs_filesystem.inode_operations.find_next_sibling = &smemfs_USB3_find_next_sibling;
	smemfs_filesystem.inode_operations.find_first_child = &smemfs_USB3_find_first_child;
	smemfs_filesystem.inode_operations.find_parent = &smemfs_USB3_find_parent;
	smemfs_filesystem.inode_operations.get_name = &smemfs_USB3_get_name;
	smemfs_filesystem.inode_operations.get_mode = &smemfs_USB3_get_mode;
	atomic_stop();
}

void smemfs_initialize(void)
{
	// Start atomic operations
	atomic_start();

	// Get / check Casio SMEM sector table
	// @note:
	// 	Casio SMEM sector table start
	// always (?) at 0xa0270000 (tested with OS 1.00.0000).
	// TODO: return error !!
	smemfs_USB2_superblock.sector_table = (void *)0xa0270000;
	if (smemfs_USB2_superblock.sector_table->magic_start != CASIO_SMEM_BLOCK_ENTRY_MAGIC)
	{
		earlyterm_write("SMEMFS: sector table error !\n");
		return (use_dump_smem_driver());
	}

	// Try to find Casio SMEM inode table start always at the end of
	// the sector table. Normaly start at 0xa0270320 but not always (?)
	int i = -1;
	while (smemfs_USB2_superblock.sector_table[++i].magic_start == CASIO_SMEM_BLOCK_ENTRY_MAGIC);

	// Get the inode table
	// TODO: return error !!
	smemfs_USB2_superblock.inode_table = (void *)&smemfs_USB2_superblock.sector_table[i];
	if ((smemfs_USB2_superblock.inode_table->info != 0x51 &&
			smemfs_USB2_superblock.inode_table->info != 0x01) ||
			smemfs_USB2_superblock.inode_table->parent.id != 0xffff ||
			smemfs_USB2_superblock.inode_table->parent.type != 0xffff)
	{
		earlyterm_write("SMEMFS: Casio inode table error !");
		return (use_dump_smem_driver());
	}

	// Stop atomic operations
	atomic_stop();
}
