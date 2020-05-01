#include <kernel/fs/vfs.h>
#include <kernel/devices/earlyterm.h>

ssize_t vfs_read(FILE *file, void *buf, size_t count)
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
	if (file->file_op->read == NULL)
		return (-4);

	// Read with FS specifique primitive and return the numbe of reading bytes.
	ssize_t read = file->file_op->read(((struct dentry*)file->private)->inode, buf, count, file->cursor);
	if (read > 0)
		file->cursor = file->cursor + read;
	return (read);
}
