#include <kernel/fs/smemfs.h>
#include <kernel/fs/stat.h>
#include <kernel/util/atomic.h>

/* smemfs_get_mode() - Return the permission dans the type of a file (sync) */
mode_t smemfs_get_mode(void *inode)
{
	extern struct smemfs_superblock_s smemfs_superblock;
	struct casio_smem_header_s *header;
	mode_t inode_type;

	// Check error
	if (inode == NULL)
		return (-1);

	// Start atomic opeartion
	atomic_start();

	// Check root inode
	if (inode == smemfs_superblock.sector_table)
	{
		atomic_stop();
		return (__S_IFDIR | S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH | S_IWOTH);
	}

	// Check inode validity
	header = inode;
	if (header->info != CASIO_SMEM_HEADER_INFO_EXIST)
	{
		atomic_stop();
		return (-1);
	}

	// We can only check if the file is a
	// directory or not (Bfile limitation)
	inode_type = (header->type == CASIO_SMEM_HEADER_TYPE_DIRECTORY) ? __S_IFDIR : __S_IFREG;

	// Stop atomic operation
	atomic_stop();

	// Return default permission and type
	return (inode_type | S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH | S_IWOTH);
}
