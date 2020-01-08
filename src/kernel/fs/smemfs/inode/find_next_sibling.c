#include <kernel/fs/smemfs.h>
#include <kernel/atomic.h>
#include <kernel/util.h>

/* smemfs_find_next_sibling() - Find the next file from the same parent (sync) */
void *smemfs_find_next_sibling(void *inode)
{
	extern struct smemfs_superblock_s smemfs_superblock;
	void *sibling_inode;
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
	
	// Return the next file of the directory.
	sibling_inode = smemfs_walk(inode, inode, folder_id, WALK_FLAG_ID_CHECK_PARENT);

	// Stop atomic operation
	atomic_stop();
	return (sibling_inode);
}
