#include <kernel/fs/smemfs.h>
#include <kernel/fs/file.h>
#include <kernel/devices/earlyterm.h>
#include <kernel/util/atomic.h>
#include <lib/string.h>

/* casio_smem_data_base_address() - Generate the fragmented data address (0xa0000000 + offset) */
static void *casio_smem_get_data_base_address(smemfs_fragdata_t *fragment)
{
	extern struct smemfs_superblock_s smemfs_superblock;
	struct casio_smem_block_s *block;

	// Find the appropriate block
	block = smemfs_superblock.sector_table;
	while (block->magic_start == CASIO_SMEM_BLOCK_ENTRY_MAGIC &&
		block->info.id != fragment->data_block_id)
	{
		block = &block[1];
	}
	// If the block ID is missing, return error
	if (block->info.id != fragment->data_block_id)
		return (NULL);
	return ((void *)(uintptr_t)(block->offset + fragment->data_offset));
}

/* casio_smem_read() - Read the file data (based on internal cursor) */
ssize_t smemfs_read(void *inode, void *buf, size_t count, off_t pos)
{
	smemfs_fragdata_t *fragment;
	smemfs_header_t *header;
	off_t fragment_data_offset;
	void *data_base_addr;
	size_t current_size;
	size_t real_size;

	//earlyterm_write("pos = %#x\n", pos);
	//DBG_WAIT;

	// Get Check obvious error.
	if (inode == NULL || buf == NULL)
		return (-1);

	// Check file type
	header = inode;
	if (header->info != CASIO_SMEM_HEADER_INFO_EXIST ||
			header->type != CASIO_SMEM_HEADER_TYPE_FILE) {
		earlyterm_write("smemfs: header error !\n");
		return (-1);
	}

	// Start atomic operation
	atomic_start();

	// Get the current data fragment.
	current_size = 0;
	fragment = (void *)&header[1];
	while (fragment->magic == CASIO_SMEM_FRAGMENT_MAGIC &&
			fragment->info == CASIO_SMEM_FRAGMENT_INFO_EXIST &&
			(off_t)(current_size + fragment->data_size + 1) < pos)
	{
		current_size = current_size + fragment->data_size + 1;
		fragment = &fragment[1];
	}

	// Check fragment error
	if (fragment->magic != CASIO_SMEM_FRAGMENT_MAGIC ||
			fragment->info != CASIO_SMEM_FRAGMENT_INFO_EXIST)
	{
		atomic_stop();
		earlyterm_write("smemfs: fragment error !\n");
		earlyterm_write("* current_size = %d\n", current_size);
		earlyterm_write("* pos = %#x\n", pos);
		earlyterm_write("* frag magic   = 0x%x\n", fragment->magic);
		earlyterm_write("* frag info    = 0x%x\n", fragment->info);
		DBG_WAIT;
		DBG_WAIT;
		DBG_WAIT;
		return (-1);
	}

	// Read file data
	fragment_data_offset = pos - current_size;
	current_size = 0;
	while (current_size < count &&
			fragment->magic == CASIO_SMEM_FRAGMENT_MAGIC &&
			fragment->info == CASIO_SMEM_FRAGMENT_INFO_EXIST)
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
		if (fragment_data_offset != 0) {
			data_base_addr = data_base_addr + fragment_data_offset;
			fragment_data_offset = 0;
		}

		// Fill the buffer
		memcpy(buf + current_size, (void*)(0xa0000000 + data_base_addr), real_size);

		// Update informations.
		current_size = current_size + real_size;
		fragment = &fragment[1];
	}

	// Stop atomic operation
	atomic_stop();
	return (current_size);
}
