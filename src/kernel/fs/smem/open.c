#include <kernel/fs/smem.h>
#include <kernel/fs/file.h>
#include <kernel/util.h>

//FIXME: find better way to walk into fragment (use ID)
static size_t casio_smem_get_file_size(casio_smem_header_t *file)
{
	casio_smem_fragment_t *fragment;
	size_t count;

	count = 0;
	fragment = (void *)((uint32_t)file + sizeof(casio_smem_header_t));
	while (fragment->magic == CASIO_SMEM_FRAGMENT_MAGIC)
	{
		count = count + fragment->data_size + 1;
		fragment = fragment + 1;
	}
	return (count);
}

/* casio_smem_get_file() - Walk into the Casio's File System and search the target file */
static casio_smem_header_t *casio_smem_get_file(char const *name, uint16_t *parent_id)
{
	extern casio_smem_header_t *casio_smem_header_table;
	casio_smem_fragment_t *fragment;
	casio_smem_header_t *header;
	int i;

	// Walk into Casio SMEM File System.
	header = casio_smem_header_table;
	while (header->info == CASIO_SMEM_HEADER_INFO_EXIST ||
			header->info == CASIO_SMEM_HEADER_INFO_DELETE)
	{
		// Check new file validity.
		if (header->info == CASIO_SMEM_HEADER_INFO_EXIST &&
				header->parent.id == *parent_id)
		{
			// Compare File name.
			i = -1;
			while (++i < 12 &&
					header->name[i] != 0x0000 &&
					header->name[i] != 0xffff &&
					(header->name[i] & 0x00ff) == name[i]);

			// Check error.
			if (name[i] == '\0' &&
				(header->name[i] == 0x0000 ||
				 header->name[i] == 0xffff))
			{
				*parent_id = header->id;
				return (header);
			}
		}

		// We need to skip fragmented data
		fragment = (void *)((uint32_t)header + sizeof(casio_smem_header_t));
		if (fragment->magic == CASIO_SMEM_FRAGMENT_MAGIC)
		{
			// The fastest way to skip fragmented data
			fragment = (void *)((uint32_t)fragment +
					(sizeof(casio_smem_fragment_t) * fragment->frag_total));
		}

		// Get next header.
		header = (void *)fragment;
	}
	return (NULL);
}

/* casio_smem_open() - Find target file and setup abstraction file structure */
int casio_smem_open(FILE *file, char const *name, int mode)
{
	extern uint16_t current_parent_id;
	casio_smem_header_t *file_raw;
	uint16_t parent_id;
	char file_name[14];
	int slot_number;
	int i;

	// check error.
	if (mode != O_RDONLY || name == NULL)
		return (-1);

	// Get parent ID.
	if (*name == '/')
	{
		parent_id = CASIO_SMEM_ROOT_ID;
		name += 1;
	} else {
		parent_id = current_parent_id;
	}

	// File Systeme walk entry.
	while (*name != '\0')
	{
		// Get current file name.
		i = -1;
		while (name[++i] != '\0' && name[i] != '/' && i < 14)
			file_name[i] = name[i];

		// Check file name validity.
		if (i >= 12 && name[i] != '\0' && name[i] != '/')
			return (-1);

		// Update internal informations.
		file_name[i] = '\0';
		name = name + i;

		// find the file.
		file_raw = casio_smem_get_file(file_name, &parent_id);
		if (file_raw == NULL)
			return (-1);

		// Directory check
		if (name[0] == '/')
		{
			if (file_raw->type != CASIO_SMEM_HEADER_TYPE_DIRECTORY)
				return (-2);
			name = name + 1;
		}
	}

	kvram_clear();
	kvram_print(0, 0, "CUL !");
	kvram_display();
	while (1);

	// Initilaize fiel object
	// TODO: Use VFS to do this !!
	file->cursor = 0;
	file->mode = mode;
	file->abstract = file_raw;
	file->size = casio_smem_get_file_size(file_raw);
	return (0);
}
