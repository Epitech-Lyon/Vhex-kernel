#include <kernel/fs/gladfs.h>
#include <kernel/atomic.h>
#include <kernel/util.h>

// Internal helper
extern struct gladfs_fragment_data_s **gladfs_file_pos(off_t *offset, struct gladfs_inode_s *inode, off_t pos);

ssize_t gladfs_read(void *inode, void *buf, size_t count, off_t pos)
{
	struct gladfs_fragment_data_s **fragdata;
	uint8_t get_next_block;
	size_t current_size;
	size_t write_size;
	off_t offset;

	// Start atomic operations
	atomic_start();

	// Get appropriate data fragment
	fragdata = gladfs_file_pos(&offset, inode, pos);
	if (fragdata == NULL || *fragdata == NULL)
		return (-1);

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
			(void *)((uint32_t)buf + current_size),
			(void *)((uint32_t)(&(*fragdata)->data_entry) + offset),
			write_size
		);

		// Update current size and wipe offset
		current_size = current_size + write_size;
		offset = 0;

		// Get next block if needed
		if (get_next_block == 1)
		{
			// Get next block
			fragdata = (void *)&(*fragdata)->next;
			if (*fragdata == NULL)
				break;
		}
	}

	// End atomic operations
	atomic_end();

	// Return read bytes
	return (current_size);
}
