#include <kernel/fs/vfs.h>
#include <kernel/fs/file.h>
#include <fcntl.h>

/* vfs_lseek() - File cursor management */
off_t vfs_lseek(FILE *file, off_t offset, int whence)
{
	// Check potential error.
	if (file == NULL)
		return (-1);

	if (whence == SEEK_SET)
	{
		file->cursor = offset;
		return (file->cursor);
	}
	if (whence == SEEK_CUR)
	{
		file->cursor = file->cursor + offset;
		return (file->cursor);
	}
	if (whence == SEEK_END)
	{
		//TODO: get file size !!
		//TODO: use copy-on-write system !!
		//file->cursor = file->size + offset - 1;
		return (-1);
	}
	return (-1);
}
