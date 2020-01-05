#include <kernel/fs/vfs.h>
#include <kernel/fs/stat.h>
#include <kernel/util.h>

// Intrenal functions
extern struct dentry *vfs_dentry_resolve(const char *path, int mode);

int vfs_open(FILE *file, char const *path, int flags)
{
	struct dentry *dentry;

	// Get target inode.
	dentry = vfs_dentry_resolve(path, 0);
	if (dentry == NULL)
		return (-1);

	// Check directory.
	if ((dentry->mode & __S_IFDIR) != 0)
		return (-2);

	// Initialize new file.
	file->private = dentry->inode;
	file->permission = dentry->mode & (~__S_IFMT);
	file->file_op = dentry->dentry_op.file_op;
	file->cursor = 0;
	return (0);
}
