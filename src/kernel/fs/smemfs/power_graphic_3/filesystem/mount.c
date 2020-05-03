#include <kernel/fs/smemfs.h>
#include <kernel/util/atomic.h>

/*
** smemfs_USB3_mount() - Mount the file system (sync)
** @note:
** We don't known how the file system work, so we should use
** Casio's "Bfile_*" sycalls to dump all internal informations
** to avoid OS switch (Vhex -> Casio -> Vhex)
*/
void *smemfs_USB3_mount(void)
{
	extern struct smemfs_superblock_s smemfs_superblock;
	void *root_inode = NULL;

	// Start atomic operation
	atomic_start();

	//TODO
	//TODO Use Bfile_FindFirst and Bfile_Find_Next to dump
	//TODO All internal FS informations
	//TODO

	// Stop atomic operation
	atomic_stop();

	// Return the sector table to simulate the root inode.
	return (root_inode);
}
