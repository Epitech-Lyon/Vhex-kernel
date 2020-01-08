#include <kernel/fs/smemfs.h>
#include <kernel/fs/file.h>
#include <kernel/atomic.h>
#include <kernel/util.h>

/* casio_smem_data_base_address() - Generate the fragmented data address (0xa0000000 + offset) */
static void *casio_smem_get_data_base_address(smemfs_fragdata_t *fragment)
{
	extern struct smemfs_superblock_s smemfs_superblock;
	struct casio_smem_block_s *block;

	block = smemfs_superblock.sector_table;
	while (block->magic_start == CASIO_SMEM_BLOCK_ENTRY_MAGIC &&
		block->info.id != fragment->data_block_id)
	{
		block = block + 1;
	}
	if (block->info.id != fragment->data_block_id)
		return (NULL);
	return ((void *)(block->offset + fragment->data_offset));
}

/* casio_smem_read() - Read the file data (based on internal cursor) */
ssize_t smemfs_read(void *inode, void *buf, size_t count, off_t pos)
{
	smemfs_fragdata_t *fragment;
	off_t fragment_data_offset;
	void *data_base_addr;
	ssize_t current_size;
	size_t real_size;

	// Get Check obvious error.
	if (inode == NULL || buf == NULL)
		return (-1);

	// Start atomic operation
	atomic_start();

	// Get the current data fragment.
	current_size = 0;
	fragment = (void *)((uint32_t)inode + sizeof(struct casio_smem_header_s));
	while (fragment->magic == CASIO_SMEM_FRAGMENT_MAGIC &&
			pos > (off_t)(current_size + fragment->data_size + 1))
	{
		current_size = current_size + fragment->data_size + 1;
		fragment = fragment + 1;
	}

	// Check fragment error
	if (fragment->magic != CASIO_SMEM_FRAGMENT_MAGIC)
	{
		atomic_stop();
		return (-1);
	}

	// Read file data
	current_size = 0;
	fragment_data_offset = pos - current_size;
	while (current_size < (ssize_t)count &&
			fragment->magic == CASIO_SMEM_FRAGMENT_MAGIC)
	{
		// Calculate the real size to read.
		real_size = fragment->data_size + 1 - fragment_data_offset;
		if (real_size >= count - current_size)
			real_size = count - current_size;

		// Get the data address.
		data_base_addr = casio_smem_get_data_base_address(fragment);
		if (data_base_addr == NULL)
			break;

		// Handle fragment data offset.
		if (fragment_data_offset != 0)
		{
			data_base_addr = (void *)(((uint32_t)data_base_addr) + fragment_data_offset);
			fragment_data_offset = 0;
		}

		// Fill the buffer
		memcpy((void*)(buf + current_size), (void*)(0xa0000000 + data_base_addr), real_size);

		// Update informations.
		current_size = current_size + real_size;
		fragment = fragment + 1;
	}

	// Stop atomic operation
	atomic_stop();
	return (current_size);
}
