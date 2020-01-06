#include <kernel/fs/vfs.h>
#include <kernel/fs/stat.h>
#include <kernel/util.h>

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

int vfs_mknod(const char *pathname, mode_t mode, dev_t dev)
{
	extern struct dentry *vfs_root_node;
	struct dentry *parent_dentry;
	struct device *device;
	struct dentry *file;
	char *name;


	// Get parent dentry
	parent_dentry = vfs_dentry_resolve(pathname, 1);
	if (parent_dentry == NULL)
	{
		kvram_clear();
		printk(0, 0, "mknod: parent dentry error !");
		kvram_display();
		DBG_WAIT;
		return (-1);
	}

	// Get folder name
	name = strrchr(pathname, '/');
	name = (name == NULL) ? (void *)pathname : &name[1];
	
	// Try to find open device
	device = device_get(dev_get_major(dev));
	if (device == NULL)
	{
		kvram_clear();
		printk(0, 0, "mknod: device error !");
		kvram_display();
		DBG_WAIT;
		return (-2);
	}

	// Tru to create empty node
	file = vfs_dentry_alloc(name, mode | __S_IFCHR);
	if (file == NULL)
	{
		kvram_clear();
		printk(0, 0, "mknod: dentry alloc error !");
		kvram_display();
		DBG_WAIT;
		return (-3);
	}

	// Try to open device
	file->inode = device->open(dev_get_major(dev), dev_get_minor(dev));
	if (file->inode == NULL)
	{
		kvram_clear();
		printk(0, 0, "mknod: device inode error !");
		kvram_display();
		DBG_WAIT;
		vfs_dentry_free(file);
		return (-4);
	}

	// Set file operations
	file->dentry_op.file_op = &device->file_op;

	// Add file into VFS
	file->next = parent_dentry->child;
	parent_dentry->child = file;
	return (0);
}
