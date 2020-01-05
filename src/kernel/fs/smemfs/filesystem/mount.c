#include <kernel/fs/smemfs.h>

/* casio_smem_mount() - Get Block and Preheader Table addresses */
void *smemfs_mount(void)
{
	extern struct smemfs_superblock_s smemfs_superblock;

	// Return the sector table to simulate the root inode.
	return ((void*)smemfs_superblock.sector_table);
}
