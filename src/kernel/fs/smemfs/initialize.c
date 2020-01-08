#include <kernel/fs/smemfs.h>
#include <kernel/fs/filesystem.h>
#include <kernel/util.h>

// Kernel FS block
struct file_system_type smemfs_filesystem =
{
	// internal informations
	.fs_name	= "smemfs",
	.fs_flags	= FS_RDONLY,

	// FS specific openrations	
	.filesystem_operations = {
		.mount  = &smemfs_mount,
		.umount = &smemfs_umount
	},

	// File operations
	.file_operations = {
		.read  = &smemfs_read,
		.write = NULL
	},

	// Inode operations
	.inode_operations = {
		// Walk
		.find_next_sibling	= &smemfs_find_next_sibling,
		.find_first_child	= &smemfs_find_first_child,
		.find_parent		= &smemfs_find_parent,

		// Memory
		.create			= NULL,
		.mkdir			= NULL,
		.rmdir			= NULL,
		.rename			= NULL,

		// Informations
		.get_name		= &smemfs_get_name,
		.get_mode		= &smemfs_get_mode
	}
};

struct smemfs_superblock_s smemfs_superblock;

void smemfs_initialize(void)
{
	// Get / check Casio SMEM sector table
	// @note:
	// 	Casio SMEM sector table start
	// always at 0xa0270000.
	smemfs_superblock.sector_table = (void *)0xa0270000;
	if (smemfs_superblock.sector_table->magic_start != 0x4200)
	{
		kvram_clear();
		printk(0, 0, "SMEMFS: Casio sector table error !");
		printk(0, 1, "Wait manual reset...");
		kvram_display();
		while (1){ __asm__ volatile ("sleep"); }
	}

	// Casio SMEM inode table start always at the end of the sector table.
	// Normaly start at 0xa0270320 but not always (?)
	int i = -1;
	while (smemfs_superblock.sector_table[++i].magic_start == CASIO_SMEM_BLOCK_ENTRY_MAGIC);
	smemfs_superblock.inode_table = (void *)&smemfs_superblock.sector_table[i];
	if ((smemfs_superblock.inode_table->info != 0x51 &&
			smemfs_superblock.inode_table->info != 0x01) ||
			smemfs_superblock.inode_table->parent.id != 0xffff ||
			smemfs_superblock.inode_table->parent.type != 0xffff)
	{
		kvram_clear();
		printk(0, 0, "SMEMFS: Casio inode table error !");
		printk(0, 1, "Wait manual reset...");
		kvram_display();
		while (1){ __asm__ volatile ("sleep"); }
	}
}
