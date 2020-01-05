#include <kernel/fs/smemfs.h>
#include <kernel/util.h>

casio_smem_header_t *smemfs_walk(casio_smem_header_t *current_inode, uint16_t parent_id, int skip)
{
	struct casio_smem_fragment_s *current_fragment;

	// Check current inode validity.
	if (current_inode == NULL)
		return (NULL);

	// Walk entry
	while (current_inode->info == CASIO_SMEM_HEADER_INFO_EXIST ||
			current_inode->info == CASIO_SMEM_HEADER_INFO_DELETE)
	{
		// New inode validity check.
		if (current_inode->info == CASIO_SMEM_HEADER_INFO_EXIST &&
				((skip & 0x01) == 0) &&
				(((skip & 0x02) == 0 && current_inode->parent.id == parent_id) ||
				((skip & 0x02) != 0  && current_inode->id == parent_id)))
		{
			return (current_inode);
		}

		// Fast fragmentation skip
		current_fragment = (void *)((uint32_t)(current_inode) + sizeof(struct casio_smem_header_s));
		if (current_fragment->magic == CASIO_SMEM_FRAGMENT_MAGIC)
		{
			current_fragment = (void *)((uint32_t)current_fragment +
				sizeof(struct casio_smem_fragment_s) * 
				current_fragment->frag_total);
		}

		// Update current inode
		current_inode = (void*)current_fragment;

		// Workaround update
		skip = skip & ~0x01;

	} 
	return (NULL);
}
