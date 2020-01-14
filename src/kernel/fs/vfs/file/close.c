#include <kernel/fs/vfs.h>

int vfs_close(FILE *file)
{
	// Check error
	if (file == NULL)
		return (-1);

	//TODO: call close primitive for device !

	// Update internal dentry counter
	((struct dentry*)file->private)->counter += 1;
	return (0);
}
