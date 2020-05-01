#include <kernel/fs/vfs.h>
#include <kernel/fs/stat.h>
#include <kernel/devices/earlyterm.h>
#include <kernel/util/atomic.h>

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
		earlyterm_write("VFS_open() path error !\n");
		DBG_WAIT;
		return (-1);
	}

	// Start atomic operations
	atomic_start();

	// Check directory.
	if ((dentry->mode & __S_IFDIR) != 0)
	{
		earlyterm_write("VFS_open(): file type error '%s'\n", pathname);
		DBG_WAIT;
		atomic_stop();
		return (-2);
	}

	// Call device specific open() primitive if needed
	if ((dentry->mode & __S_IFCHR) != 0 && dentry->counter == 0) 
	{
		// Check potential error
		if (dentry->device == NULL) {
			earlyterm_write("VFS_open: device does not exist\n");
			DBG_WAIT;
			atomic_stop();
			return (-3);
		}
		
		// Open the device
		dentry->inode = dentry->device->open(dev_get_major(dentry->dev_id), dev_get_minor(dentry->dev_id));
		if (dentry->inode == NULL) {
			earlyterm_write("VFS_open: device open error !!\n");
			DBG_WAIT;
			atomic_stop();
			return (-4);
		}
	}
	
	// Update interne dentry counter 
	dentry->counter = dentry->counter + 1;

	// Initialize new file.
	file->private = dentry;
	file->permission = dentry->mode & (~__S_IFMT);
	file->file_op = dentry->dentry_op.file_op;
	file->cursor = 0;

	// Stop atomic operations
	atomic_stop();
	return (0);
}
