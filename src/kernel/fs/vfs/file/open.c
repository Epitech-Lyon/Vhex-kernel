#include <kernel/fs/vfs.h>
#include <kernel/fs/stat.h>
#include <kernel/util.h>

/* vfs_open() - Open file named pathname */
int vfs_open(FILE *file, char const *pathname, int flags)
{
	struct dentry *dentry;

	// Get target inode.
	dentry = vfs_dentry_resolve(pathname, 0);
	if (dentry == NULL)
	{
		kvram_clear();
		printk(0, 0, "VFS_open() error !");
		printk(0, 1, "path error '%s'", pathname);
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

	// Debug
	kvram_clear();
	printk(0, 0, "vfs_open(): inode found !");
	printk(0, 1, "path: %s", pathname);
	printk(0, 2, "name: %s", dentry->name);
	printk(0, 3, "inode: %p", dentry->inode);
	printk(0, 4, "file_op: %p", dentry->dentry_op.file_op);
	kvram_display();
	DBG_WAIT;

	// Update interne dentry counter 
	dentry->counter = dentry->counter + 1;

	// Initialize new file.
	file->private = dentry;
	file->permission = dentry->mode & (~__S_IFMT);
	file->file_op = dentry->dentry_op.file_op;
	file->cursor = 0;
	return (0);
}
