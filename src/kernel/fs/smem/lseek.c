#include <kernel/fs/smem.h>
#include <kernel/fs/file.h>

/* casio_smem_lseek() - File cursor management */
off_t casio_smem_lseek(FILE *file, off_t offset, int whence)
{
	// Check potential error.
	if (file == NULL)
		return (-1);

	if (whence == SEEK_SET){
		file->cursor = offset;
		return (file->cursor);
	}
	if (whence == SEEK_CUR){
		file->cursor = file->cursor + offset;
		return (file->cursor);
	}
	if (whence == SEEK_END){
		file->cursor = file->size + offset - 1;
		return (file->cursor);
	}
	return (-1);
}
