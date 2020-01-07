#include <kernel/fs/smemfs.h>
#include <kernel/util.h>

void *smemfs_find_next_sibling(void *inode)
{
	extern struct smemfs_superblock_s smemfs_superblock;
	uint16_t folder_id;

	// Check error.
	if (inode == NULL)
		return (NULL);

	// Check root inode
	if (inode == smemfs_superblock.sector_table)
		return (NULL);

	// Check inode validity
	if (((struct casio_smem_header_s *)inode)->info != CASIO_SMEM_HEADER_INFO_EXIST)
		return (NULL);

	// Get parent ID.
	folder_id = ((struct casio_smem_header_s *)inode)->parent.id;
	
	// Return the next file of the directory.
	return (smemfs_walk(inode, inode, folder_id, WALK_FLAG_ID_CHECK_PARENT));
}
