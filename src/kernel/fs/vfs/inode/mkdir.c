#include <kernel/fs/vfs.h>
#include <kernel/memory.h>
#include <kernel/fs/stat.h>
#include <lib/string.h>

/* vfs_mkdir() - Attempts to create a directory named pathname */
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
	parent_dentry = vfs_dentry_resolve(pathname, VFS_DENTRY_RESOLVE_FLAG_PATHNAME);
	
	// Check parent dentry and FS primitives
	if (parent_dentry == NULL || parent_dentry->dentry_op.inode_op->mkdir == NULL)
		return (-1);

	// Try to create new dentry
	folder = vfs_dentry_alloc(name, mode | __S_IFDIR);
	if (folder == NULL)
		return (-2);

	// Try to request new folder
	folder->inode = parent_dentry->dentry_op.inode_op->mkdir(parent_dentry->inode, name, mode);
	if (folder->inode == NULL)
	{
		vfs_dentry_free(folder);
		return (-3);
	}

	// Set FS primitives operations
	folder->dentry_op.file_op = parent_dentry->dentry_op.file_op;
	folder->dentry_op.inode_op = parent_dentry->dentry_op.inode_op;

	// Release dentry
	folder->parent = parent_dentry;
	folder->next = parent_dentry->child;
	parent_dentry->child = folder;
	return (0);
}
