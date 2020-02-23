#include <kernel/fs/smemfs.h>
#include <kernel/util/atomic.h>

/* smemfs_find_parent() - Return the parent inode */
void *smemfs_find_parent(void *inode)
{
	extern struct smemfs_superblock_s smemfs_superblock;
	void *parent_inode;
	uint16_t folder_id;

	// Check error.
	if (inode == NULL)
		return (NULL);

	// Start atomic operation
	atomic_start();

	// Check inode validity (and root)
	if (inode == smemfs_superblock.sector_table ||
		((struct casio_smem_header_s *)inode)->info != CASIO_SMEM_HEADER_INFO_EXIST)
	{
		atomic_stop();
		return (NULL);
	}

	// Get parent ID.
	folder_id = ((struct casio_smem_header_s *)inode)->parent.id;

	// Return first inode find
	parent_inode = smemfs_walk(inode, smemfs_superblock.inode_table, folder_id,
						WALK_FLAG_ID_CHECK_DIRECTORY);

	// Stop atomic operation
	atomic_stop();
	return (parent_inode);
}
