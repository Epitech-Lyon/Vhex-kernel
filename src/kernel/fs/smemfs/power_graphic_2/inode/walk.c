#include <kernel/fs/smemfs.h>

/* smemfs_walk() - Find inode based on directory ID and flags */
/* @note: This function is internal of smemFS, do not call it ! */
smemfs_inode_t *smemfs_USB2_walk(smemfs_inode_t *current,
				smemfs_inode_t *file, uint16_t folder_id, int flags)
{
	smemfs_fragdata_t *fragdata;

	// Check current inode validity.
	if (file == NULL)
		return (NULL);

	// Walk file
	while (file->info == CASIO_SMEM_HEADER_INFO_EXIST ||
			file->info == CASIO_SMEM_HEADER_INFO_DELETE)
	{
		// New inode validity check.
		if (file != current &&
			file->info == CASIO_SMEM_HEADER_INFO_EXIST &&
			(((flags & WALK_FLAG_ID_CHECK_PARENT) != 0 && file->parent.id == folder_id) ||
			((flags & WALK_FLAG_ID_CHECK_PARENT) == 0  && file->id == folder_id)))
		{
			return (file);
		}

		// Fast fragmentation skip
		fragdata = (void *)&file[1];
		if (fragdata->magic == CASIO_SMEM_FRAGMENT_MAGIC)
			fragdata = &fragdata[fragdata->frag_total];

		// Update current inode
		file = (void*)fragdata;
	} 
	return (NULL);
}
