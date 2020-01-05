#include <kernel/fs/vfs.h>
#include <kernel/memory.h>
#include <kernel/util.h>
#include <kernel/fs/stat.h>

// Internal informations
struct dentry *vfs_root_node = NULL;

// filesystem_get() - Get FS registered by the VFS
static struct file_system_type *filesystem_get(const char *name)
{
	extern struct file_system_type *fs_list;
	struct file_system_type *filesystem;

	filesystem = fs_list;
	while (filesystem != NULL)
	{
		// Check file system
		if (strcmp(filesystem->fs_name, name) == 0)
			return (filesystem);
		
		// Get next file system
		filesystem = filesystem->next;
	}
	return (NULL);
}

/* vfs_mount() - mount filesystem */
int vfs_mount(const char *source, const char *target,
		const char *filesystemtype, unsigned long mountflags,
		const void *data)
{
	// Check error.
	if (filesystemtype == NULL)
		return (-1);

	// Get file system informations
	struct file_system_type *filesystem = filesystem_get(filesystemtype);
	if (filesystem == NULL)
		return (-1);

	// Check ROOT mount.
	if (target == NULL && mountflags == (unsigned long)VFS_MOUNT_ROOT)
	{
		// Alloc root mnt
		vfs_root_node = pm_alloc(sizeof(struct dentry));
		if (vfs_root_node == NULL)
			return (-1);
		
		// Try to mount the FS.
		vfs_root_node->inode = filesystem->filesystem_operations.mount();
		if (vfs_root_node->inode == NULL)
		{
			kvram_clear();
			printk(0, 0, "VFS: Unable to mount ROOT inode !");
			printk(0, 1, "VFS: Wait manual reset...");
			kvram_display();
			while (1) { __asm__ volatile ("sleep"); }
		}

		// Initialize entry
		vfs_root_node->name[0] = '/';
		vfs_root_node->name[1] = '\0';
		vfs_root_node->parent = NULL;
		vfs_root_node->child = NULL;
		vfs_root_node->next = NULL;
		vfs_root_node->dentry_op.file_op = &filesystem->file_operations;
		vfs_root_node->dentry_op.inode_op = &filesystem->inode_operations;
		vfs_root_node->mnt.inode = NULL;
		vfs_root_node->mnt.file_op = NULL;
		vfs_root_node->mnt.inode_op = NULL;
		return (0);
	}

	// Get target inode informations
	struct dentry *mnt = vfs_dentry_resolve(target, 0);
	if (mnt == NULL)
		return (-1);

	// Check directory validity
	if ((mnt->mode & __S_IFDIR) == 0)
		return (-2);

	// Try to mount the directory
	void *inode = filesystem->filesystem_operations.mount();
	if (inode == NULL)
		return (-3);

	// Init mount point: set new FS specific primitives
	// TODO: set new FS flags
	mnt->mnt.inode = inode;
	mnt->mnt.file_op = &filesystem->file_operations;
	mnt->mnt.inode_op = &filesystem->inode_operations;
	return (0);
}
