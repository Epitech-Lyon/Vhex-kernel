#include <kernel/fs/gladfs.h>
#include <kernel/memory.h>

struct gladfs_fragment_data_s **gladfs_file_pos(off_t *offset, struct gladfs_inode_s *inode, off_t pos)
{
	struct gladfs_fragment_data_s **fragdata;
	uint32_t current_offset;

	// Check first fragment.
	fragdata = &inode->fragdata;
	if (*fragdata == NULL)
	{
		*offset = -1;
		return (fragdata);
	}

	// Check pos
	if (pos > inode->size)
		pos = inode->size;

	// Get fragment data
	current_offset = 0;
	while (current_offset + (*fragdata)->data_used < pos)
	{
		current_offset = current_offset + (*fragdata)->data_used;
		fragdata = (void *)&(*fragdata)->next;
	}

	// Get offset
	*offset = pos - current_offset;

	// Return fragdata
	return (fragdata);
}
