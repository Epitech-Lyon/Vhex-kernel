#include <kernel/fs/vfs.h>
#include <kernel/fs/stat.h>
#include <kernel/devices/earlyterm.h>

/* vfs_open() - Open file named pathname */
int vfs_open(FILE *file, char const *pathname, int flags)
{
	struct dentry *dentry;

	//TODO: handle flags !!
	(void)flags;

	// Get target inode.
	dentry = vfs_dentry_resolve(pathname, 0);
	if (dentry == NULL)
	{
		earlyterm_write("VFS_open() error !\n");
		earlyterm_write("* path error '%s'\n", pathname);
		DBG_WAIT;
		return (-1);
	}

	// Check directory.
	if ((dentry->mode & __S_IFDIR) != 0)
	{
		earlyterm_write("VFS_open(): file type error '%s'\n", pathname);
		DBG_WAIT;
		return (-2);
	}

	//TODO: call FS specific open() primitive ?

	// Debug
/*	earlyterm_write("vfs_open(): inode found !");
	earlyterm_write("* path: %s", pathname);
	earlyterm_write("* name: %s", dentry->name);
	earlyterm_write("* inode: %p", dentry->inode);
	earlyterm_write("* file_op: %p", dentry->dentry_op.file_op);
	DBG_WAIT;*/

	// Update interne dentry counter 
	dentry->counter = dentry->counter + 1;

	// Initialize new file.
	file->private = dentry;
	file->permission = dentry->mode & (~__S_IFMT);
	file->file_op = dentry->dentry_op.file_op;
	file->cursor = 0;
	return (0);
}
