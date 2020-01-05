#include <kernel/fs/smemfs.h>
#include <kernel/util.h>

// Internal function
extern casio_smem_header_t *smemfs_walk(casio_smem_header_t *current_inode,
						uint16_t parent_id, int skip);

void *smemfs_find_next_sibling(void *inode)
{
	extern struct smemfs_superblock_s smemfs_superblock;
	uint16_t parent_id;

	// Check error.
	if (inode == NULL)
		return (NULL);

	// Check root inode
	if (inode == smemfs_superblock.sector_table)
		return (NULL);

	// Get parent ID.
	parent_id = ((struct casio_smem_header_s *)inode)->parent.id;
	
	// Return the next file of the directory.
	return (smemfs_walk(inode, parent_id, 0x01));
}
