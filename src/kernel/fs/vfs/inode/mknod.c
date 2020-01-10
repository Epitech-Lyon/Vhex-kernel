#include <kernel/fs/vfs.h>
#include <kernel/fs/stat.h>
#include <kernel/util.h>

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

	// Get parent dentry
	parent_dentry = vfs_dentry_resolve(pathname, VFS_DENTRY_RESOLVE_FLAG_PATHNAME);
	if (parent_dentry == NULL)
		return (-1);

	// Get folder name
	name = strrchr(pathname, '/');
	name = (name == NULL) ? (void *)pathname : &name[1];
	
	// Try to find the device
	// TODO: handle othe file (mode) !!!
	device = device_get(dev_get_major(dev));
	if (device == NULL)
		return (-2);

	// Tru to create empty node
	file = vfs_dentry_alloc(name, mode | __S_IFCHR);
	if (file == NULL)
		return (-3);


	// Try to open device
	file->inode = device->open(dev_get_major(dev), dev_get_minor(dev));
	if (file->inode == NULL)
		return (-4);


	// Debug !
	kvram_clear();
	printk(0, 0, "New mknod device !");
	printk(0, 1, "dentry: %p$", file);
	printk(0, 2, "inode: %p$", file->inode);
	printk(0, 3, "name: %s$", file->name);
	printk(0, 4, "dev->file_op: %p$", &device->file_op);
	kvram_display();
	DBG_WAIT;
	DBG_WAIT;

	// Set file operations
	file->dentry_op.file_op = &device->file_op;

	// Add file into VFS
	file->parent = parent_dentry;
	file->next = parent_dentry->child;
	parent_dentry->child = file;
	return (0);
}
