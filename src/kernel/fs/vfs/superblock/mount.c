#include <kernel/fs/vfs.h>
#include <kernel/memory.h>
#include <kernel/util/debug.h>
#include <kernel/util/string.h>
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
		// Check VFS root
		if (vfs_root_node != NULL)
			return (-4);

		// Alloc root mnt
		vfs_root_node = vfs_dentry_alloc("/", __S_IFDIR);
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

		// Get File System primitives
		vfs_root_node->dentry_op.file_op = &filesystem->file_operations;
		vfs_root_node->dentry_op.inode_op = &filesystem->inode_operations;

		// Debug !
		/*kvram_clear();
		printk(0, 0, "vfs_root_node = %p", vfs_root_node);
		printk(0, 1, "vfs_root_node = %s$", vfs_root_node->name);
		printk(0, 2, "vfs_root_node = %p", vfs_root_node->child);
		printk(0, 3, "vfs_root_node = %p", vfs_root_node->next);
		kvram_display();
		DBG_WAIT;*/
		return (0);
	}

	// Get target inode informations
	struct dentry *mnt = vfs_dentry_resolve(target, VFS_DENTRY_RESOLVE_FLAG_FULL_PATH);
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
