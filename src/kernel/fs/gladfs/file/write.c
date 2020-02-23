#include <kernel/fs/gladfs.h>
#include <kernel/util/atomic.h>
#include <kernel/util/string.h>

// Internal helper
extern struct gladfs_fragment_data_s **gladfs_file_pos(off_t *offset, struct gladfs_inode_s *inode, off_t pos);

/* gladfs_write() - GladFS inode write primitive (sync) */
ssize_t gladfs_write(void *inode, const void *buf, size_t count, off_t pos)
{
	extern struct gladfs_superblock_s gladfs_superblock;
	struct gladfs_fragment_data_s **fragdata;
	uint8_t get_next_block;
	size_t current_size;
	size_t write_size;
	off_t offset;

	// Check error
	if (inode == NULL)
		return (-1);

	// Start atomic operation
	atomic_start();

	// Get appropriate data fragment
	fragdata = gladfs_file_pos(&offset, inode, pos);
	if (fragdata == NULL)
	{
		atomic_stop();
		return (-1);
	}

	// Generate new indirect block if needed
	if (*fragdata == NULL)
	{
		// @note: for now, the indirect block allocation (like
		// ext2 file system) is not really implemented :(
		gladfs_superblock.super_op.alloc_fragdata(fragdata, 1);
		if (*fragdata == NULL)
		{
			atomic_stop();
			return (-1);
		}

		// Reset offset
		offset = 0;
	}

	// Walk into fragemented data
	current_size = 0;
	count = count - offset;
	while (current_size < count)
	{
		// Generate writable zone
		if ((*fragdata)->data_size - offset >= count - current_size)
		{
			write_size = count - current_size;
			get_next_block = 0;
		} else {
			write_size = (*fragdata)->data_size - offset;
			get_next_block = 1;
		}

		// Write data.
		memcpy(
			(void *)((uint32_t)(&(*fragdata)->data_entry) + offset),
			(void *)((uint32_t)buf + current_size),
			write_size
		);

		// Update fragment / inode informations
		((struct gladfs_inode_s*)inode)->size += write_size;
		(*fragdata)->data_used = (*fragdata)->data_used + write_size;
		current_size = current_size + write_size;
		offset = 0;

		// Get next block if needed
		if (get_next_block == 1)
		{
			// Get next block
			fragdata = (void *)&(*fragdata)->next;
			if (*fragdata != NULL)
				continue;

			// Generate new block
			// @note: for now, the indirect block allocation is
			// not really implemented :(
			gladfs_superblock.super_op.alloc_fragdata(fragdata, 1);
			if (*fragdata == NULL)
				break;
		}
	}

	// End atomic operations
	atomic_stop();

	// Return written bytes.
	return (current_size);
}
