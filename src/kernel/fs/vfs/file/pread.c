#include <kernel/fs/vfs.h>
#include <kernel/devices/earlyterm.h>
#include <lib/string.h>

ssize_t vfs_pread(FILE *file, void *buf, size_t count, off_t offset)
{
	//TODO: Check permission !!

	// Check error
	if (file == NULL || buf == NULL)
		return (-1);

	// Get / check file's informations
	if (file->private == NULL ||
			file->file_op == NULL ||
			file->file_op->read == NULL)
		return (-1);

	// Debug
	earlyterm_write("pos = %#x\n", offset);

	// Check error
	// Read with FS specifique primitive and return the numbe of reading bytes.
	return (file->file_op->read(((struct dentry*)file->private)->inode, buf, count, offset));
}
