#include <kernel/fs/smemfs.h>
#include <kernel/util.h>

/* smemfs_find_first_child() - Find the fist file in the (folder) inode */
void *smemfs_find_first_child(void *inode)
{
	extern struct smemfs_superblock_s smemfs_superblock;
	uint16_t folder_id;

	// Check error.
	if (inode == NULL)
		return (NULL);

	// Check root inode
	if (inode == smemfs_superblock.sector_table)
	{
		folder_id = CASIO_SMEM_ROOT_ID;
	} else {
		// Check inode validity
		if (((struct casio_smem_header_s *)inode)->info != CASIO_SMEM_HEADER_INFO_EXIST)
			return (NULL);

		// Check directory
		if (((struct casio_smem_header_s *)inode)->type != CASIO_SMEM_HEADER_TYPE_DIRECTORY)
			return (NULL);

		// Get directory ID
		folder_id = ((struct casio_smem_header_s *)inode)->id;
	}

	// Return the first child of the file.
	return (smemfs_walk(inode, smemfs_superblock.inode_table,
				folder_id, WALK_FLAG_ID_CHECK_PARENT));
}
