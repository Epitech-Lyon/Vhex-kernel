#include <kernel/fs/vfs.h>
#include <kernel/memory.h>
#include <kernel/fs/stat.h>
#include <kernel/util.h>

// Internal functions
struct dentry *vfs_dentry_resolve(const char *path, int mode);

int vfs_mkdir(const char *pathname, mode_t mode)
{
	extern struct dentry *vfs_root_node;
	struct dentry *parent_dentry;
	struct dentry *folder;
	char *name;

	// Get folder name
	name = strrchr(pathname, '/');
	name = (name == NULL) ? (void *)pathname : &name[1];

	// Get parent dentry
	parent_dentry = vfs_dentry_resolve(pathname, 1);
	
	// Check parent dentry and FS primitives
	if (parent_dentry == NULL || parent_dentry->dentry_op.inode_op->mkdir == NULL)
		return (-1);

	// Try to create new dentry
	folder = pm_alloc(sizeof(struct dentry));
	if (folder == NULL)
		return (-2);

	// Try to request new folder
	folder->inode = parent_dentry->dentry_op.inode_op->mkdir(parent_dentry->inode, name, mode);
	if (folder->inode == NULL)
	{
		pm_free(folder);
		return (-3);
	}

	// Initialize new dentry
	memset(folder->name, 0x00, VFS_DENTRY_NAME_LENGHT);
	strncpy(folder->name, name, VFS_DENTRY_NAME_LENGHT);
	folder->mode = mode | __S_IFDIR;
	folder->parent = parent_dentry;
	folder->next = parent_dentry->child;
	folder->child = NULL;
	folder->dentry_op.file_op = parent_dentry->dentry_op.file_op;
	folder->dentry_op.inode_op = parent_dentry->dentry_op.inode_op;
	folder->mnt.inode = NULL;
	folder->mnt.inode_op = NULL;
	folder->mnt.file_op = NULL;

	// Release dentry
	parent_dentry->child = folder;
	return (0);
}
