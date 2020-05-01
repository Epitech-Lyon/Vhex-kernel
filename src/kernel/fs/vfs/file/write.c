#include <kernel/fs/vfs.h>

ssize_t vfs_write(FILE *file, const void *buf, size_t count)
{
	//TODO: Check permission !!

	// Check error
	if (file == NULL || buf == NULL)
		return (-1);

	// Get / check file's informations
	if (file->private == NULL)
		return (-2);
	if (file->file_op == NULL)
		return (-3);
	if (file->file_op->write == NULL)
		return (-4);

	// Writa with FS specifique primitive and return the numbe of reading bytes.
	ssize_t write = file->file_op->write(((struct dentry*)file->private)->inode, buf, count, file->cursor);
	if (write > 0)
		file->cursor = file->cursor + write;
	return (write);
}
