#include <kernel/fs/vfs.h>
#include <kernel/fs/stat.h>
#include <kernel/util/atomic.h>
#include <lib/string.h>

/* gevice_get() - Find internal device */
struct device *device_get(dev_t major)
{
	extern uint32_t bdevice_section;
	extern uint32_t edevice_section;
	struct device *device;
	int i;

	i = -1;
	device = (void *)&bdevice_section;
	while ((uint32_t)&device[++i] < (uint32_t)&edevice_section)
	{
		if (device[i].major == major)
			return (&device[i]);
	}
	return (NULL);
}

/* vfs_mknod - Create VFS inode (file, device special file, or pipe) named pathname */
int vfs_mknod(const char *pathname, mode_t mode, dev_t dev)
{
	extern struct dentry *vfs_root_node;
	struct dentry *parent_dentry;
	struct device *device;
	struct dentry *file;
	char *name;

	// Check device
	// TODO: remove me !
	if (dev == 0)
		return (-2);

	// Start atomic operations
	atomic_start();

	// Get parent dentry
	parent_dentry = vfs_dentry_resolve(pathname, VFS_DENTRY_RESOLVE_FLAG_PATHNAME);
	if (parent_dentry == NULL) {
		atomic_stop();
		return (-1);
	}

	// Get folder name
	name = strrchr(pathname, '/');
	name = (name == NULL) ? (void *)pathname : &name[1];
	
	// Try to find the device
	// TODO: handle other file (mode) !!!
	device = device_get(dev_get_major(dev));
	if (device == NULL) {
		atomic_stop();
		return (-2);
	}

	// Try to create empty node
	file = vfs_dentry_alloc(name, mode | __S_IFCHR);
	if (file == NULL) {
		atomic_stop();
		return (-3);
	}

	// Indicate device file
	file->inode = NULL;
	file->device = device;
	file->dev_id = dev;

	// Set file operations
	file->dentry_op.file_op = &device->file_op;

	// Add file into VFS
	file->parent = parent_dentry;
	file->next = parent_dentry->child;
	parent_dentry->child = file;
	atomic_stop();
	return (0);
}
