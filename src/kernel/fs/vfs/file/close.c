#include <kernel/fs/vfs.h>
#include <kernel/fs/stat.h>
#include <kernel/util/atomic.h>

int vfs_close(FILE *file)
{
	struct dentry *dentry;

	// Check error
	if (file == NULL)
		return (-1);

	// Get the dentry
	dentry = file->private;

	// Start atomic operations
	atomic_start();

	// Update internal dentry counter
	dentry->counter = dentry->counter - 1;

	// Call close primitive for the device if needed
	if ((dentry->mode & __S_IFCHR) != 0 && dentry->counter == 0) {
		dentry->device->close(dentry->inode);
		dentry->inode = NULL;
	}

	// Wipe all inforations
	file->cursor = 0;
	file->file_op = NULL;
	file->permission = 0;
	file->private = NULL;

	// Stop atomic operations
	atomic_stop();
	return (0);
}
