#include <kernel/fs/vfs.h>

ssize_t vfs_pwrite(FILE *file, const void *buf, size_t count, off_t offset)
{
	//TODO: Check permission !!

	// Check error
	if (file == NULL || buf == NULL)
		return (-1);

	// Get / check file's informations
	if (file->private == NULL ||
			file->file_op == NULL ||
			file->file_op->write == NULL)
		return (-1);

	// Writa with FS specifique primitive and return the numbe of reading bytes.
	return (file->file_op->write(((struct dentry*)file->private)->inode, buf, count, offset));
}
