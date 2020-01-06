#include <kernel/fs/vfs.h>
#include <kernel/fs/stat.h>
#include <kernel/util.h>

// Intrenal functions
extern struct dentry *vfs_dentry_resolve(const char *path, int mode);

int vfs_open(FILE *file, char const *pathname, int flags)
{
	struct dentry *dentry;

	// Get target inode.
	dentry = vfs_dentry_resolve(pathname, 0);
	if (dentry == NULL)
	{
		kvram_clear();
		printk(0, 0, "VFS_open(): path error '%s'", pathname);
		kvram_display();
		DBG_WAIT;
		return (-1);
	}

	// Check directory.
	if ((dentry->mode & __S_IFDIR) != 0)
	{
		kvram_clear();
		printk(0, 0, "VFS_open(): file type error '%s'", pathname);
		kvram_display();
		DBG_WAIT;
		return (-2);
	}

	// Initialize new file.
	file->private = dentry->inode;
	file->permission = dentry->mode & (~__S_IFMT);
	file->file_op = dentry->dentry_op.file_op;
	file->cursor = 0;
	return (0);
}
