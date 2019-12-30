#include <kernel/fs/smem.h>
#include <kernel/fs/file.h>
#include <kernel/util.h>

/* casio_smem_data_base_address() - Generate the fragmented data address (0xa0000000 + offset) */
static void *casio_smem_get_data_base_address(casio_smem_fragment_t *fragment)
{
	extern casio_smem_block_t *casio_smem_block_table;
	casio_smem_block_t *block;

	block = casio_smem_block_table;
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
ssize_t casio_smem_read(FILE *file, void *buf, size_t count)
{
	casio_smem_fragment_t *fragment;
	off_t fragment_data_offset;
	void *data_base_addr;
	ssize_t current_size;
	size_t real_size;

	// Get Check obvious error.
	if (file == NULL || buf == NULL || file->cursor > (off_t)file->size)
		return (-1);

	// Get the "real" reading size
	if (file->size - file->cursor < count)
		count = file->size - file->cursor;

	// Get the current data fragment.
	current_size = 0;
	fragment = (void *)((uint32_t)file->abstract + sizeof(casio_smem_header_t));
	while (fragment->magic == CASIO_SMEM_FRAGMENT_MAGIC &&
			file->cursor > (off_t)(current_size + fragment->data_size + 1))
	{
		current_size = current_size + fragment->data_size + 1;
		fragment = fragment + 1;
	}
	fragment_data_offset = file->cursor - current_size;

	// Read file data
	current_size = 0;
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
			return (current_size);

		// Handle fragment data offset.
		if (fragment_data_offset != 0){
			data_base_addr = (void *)(((uint32_t)data_base_addr) + fragment_data_offset);
			fragment_data_offset = 0;
		}

		// Fill the buffer
		memcpy((void*)(buf + current_size), (void*)(0xa0000000 + data_base_addr), real_size);

		// Update informations.
		current_size = current_size + real_size;
		file->cursor = file->cursor + real_size;
		fragment = fragment + 1;
	}
	((uint8_t*)buf)[current_size] = '\0';
	return (current_size);
}
