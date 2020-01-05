#include <kernel/fs/vfs.h>
#include <kernel/util.h>

ssize_t vfs_write(FILE *file, const void *buf, size_t count)
{
	//TODO: Check permission !!

	// Check error
	if (file == NULL || buf == NULL)
		return (-1);

	// Get / check file's informations
	if (file->private == NULL || file->file_op->write == NULL)
		return (-1);

	// Writa with FS specifique primitive and return the numbe of reading bytes.
	ssize_t write = file->file_op->write(file->private, buf, count, file->cursor);
	if (write != -1)
		file->cursor = file->cursor + write;
	return (write);
}
