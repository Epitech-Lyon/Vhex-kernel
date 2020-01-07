#include <kernel/fs/smemfs.h>

/* smemfs_walk() - Find inode based on directory ID and flags */
smemfs_inode_t *smemfs_walk(smemfs_inode_t *current,
				smemfs_inode_t *entry, uint16_t folder_id, int flags)
{
	smemfs_fragdata_t *fragdata;

	// Check current inode validity.
	if (entry == NULL)
		return (NULL);

	// Walk entry
	while (entry->info == CASIO_SMEM_HEADER_INFO_EXIST ||
			entry->info == CASIO_SMEM_HEADER_INFO_DELETE)
	{
		// New inode validity check.
		if (entry != current &&
			entry->info == CASIO_SMEM_HEADER_INFO_EXIST &&
			(((flags & WALK_FLAG_ID_CHECK_PARENT) != 0 && entry->parent.id == folder_id) ||
			((flags & WALK_FLAG_ID_CHECK_PARENT) == 0  && entry->id == folder_id)))
		{
			return (entry);
		}

		// Fast fragmentation skip
		fragdata = (void *)((uint32_t)(entry) + sizeof(struct casio_smem_header_s));
		if (fragdata->magic == CASIO_SMEM_FRAGMENT_MAGIC)
		{
			fragdata = (void *)((uint32_t)fragdata +
					(sizeof(struct casio_smem_fragment_s) * fragdata->frag_total));
		}

		// Update current inode
		entry = (void*)fragdata;
	} 
	return (NULL);
}
