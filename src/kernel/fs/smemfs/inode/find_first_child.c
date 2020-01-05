#include <kernel/fs/smemfs.h>
#include <kernel/util.h>

// Internal function
extern casio_smem_header_t *smemfs_walk(casio_smem_header_t *current_inode,
						uint16_t parent_id, int skip);

void *smemfs_find_first_child(void *inode)
{
	extern struct smemfs_superblock_s smemfs_superblock;
	uint16_t parent_id;

	// Check error.
	if (inode == NULL)
		return (NULL);

	// Check root inode
	if (inode == smemfs_superblock.sector_table)
	{
		parent_id = CASIO_SMEM_ROOT_ID;
	} else {
		// Check directory
		if (((struct casio_smem_header_s *)inode)->type != CASIO_SMEM_HEADER_TYPE_DIRECTORY)
			return (NULL);

		// Get directory ID
		parent_id = ((struct casio_smem_header_s *)inode)->id;
	}

	// Return the first child of the file.
	return (smemfs_walk(smemfs_superblock.inode_table, parent_id, 0));
}
