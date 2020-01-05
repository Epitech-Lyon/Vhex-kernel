#include <kernel/fs/smemfs.h>

// Internal function
extern casio_smem_header_t *smemfs_walk(casio_smem_header_t *current_inode,
						uint16_t parent_id, int skip);

void *smemfs_find_parent(void *inode)
{
	extern struct smemfs_superblock_s smemfs_superblock;
	uint16_t parent_id;

	// Check error.
	if (inode == NULL)
		return (NULL);

	// Get parent ID.
	parent_id = ((struct casio_smem_header_s *)inode)->parent.id;

	// Return first inode find
	return (smemfs_walk(smemfs_superblock.inode_table, parent_id, 0x01 | 0x02));
}
