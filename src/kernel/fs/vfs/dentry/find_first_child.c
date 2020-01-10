#include <kernel/fs/vfs.h>
#include <kernel/memory.h>
#include <kernel/util.h>
#include <kernel/fs/stat.h>

//
// vfs_dentry_find_first_child()
// Find the fist file of a file
// @note:
// 	To avoid useless memory used, the VFS cache
// is generated only if the user want to access to a file
// unregister to the cahe.
// TODO: explain correctly x)
//
struct dentry *vfs_dentry_find_first_child(struct dentry *dentry)
{
	// Check file type
	if (dentry == NULL || (dentry->mode & __S_IFDIR) == 0)
		return (NULL);

	// Try to find next sibling into the VFS cache
	if (dentry->child != NULL)
		return (dentry->child);

	// Check internal primitives
	if (dentry->dentry_op.inode_op == NULL ||
		dentry->dentry_op.inode_op->find_first_child == NULL ||
		dentry->dentry_op.inode_op->get_mode == NULL ||
		dentry->dentry_op.inode_op->get_name == NULL)
		return (NULL);

	// Try to get first child inode
	int mounted = 0;
	void *inode = dentry->dentry_op.inode_op->find_first_child(dentry->inode);
	if (inode == NULL)
	{
		// Check mnt point
		if (dentry->mnt.inode == NULL ||
			dentry->mnt.inode_op == NULL ||
			dentry->mnt.inode_op->find_first_child == NULL ||
			dentry->mnt.inode_op->get_mode == NULL ||
			dentry->mnt.inode_op->get_name == NULL)
			return (NULL);

		// Try to get mounted child
		inode = dentry->mnt.inode_op->find_first_child(dentry->mnt.inode);
		if (inode == NULL)
			return (NULL);

		// Indicate mounted point
		mounted = 1;
	}

	// Try to create new dentry
	struct dentry *new_dentry = vfs_dentry_alloc(NULL, 0);
	if (new_dentry == NULL)
		return (NULL);

	// Initialize new dentry
	if (mounted == 0)
	{
		dentry->dentry_op.inode_op->get_name(inode, new_dentry->name, VFS_DENTRY_NAME_LENGHT);
		new_dentry->mode = dentry->dentry_op.inode_op->get_mode(inode);
		new_dentry->dentry_op.inode_op = dentry->dentry_op.inode_op;
		new_dentry->dentry_op.file_op = dentry->dentry_op.file_op;
	} else {
		dentry->mnt.inode_op->get_name(inode, new_dentry->name, VFS_DENTRY_NAME_LENGHT);
		new_dentry->mode = dentry->mnt.inode_op->get_mode(inode);
		new_dentry->dentry_op.inode_op = dentry->mnt.inode_op;
		new_dentry->dentry_op.file_op = dentry->mnt.file_op;
	}

	// Common init
	new_dentry->inode = inode;
	new_dentry->parent = dentry;
	new_dentry->next = dentry->child;

	// Update VFS cache en return
	dentry->child = new_dentry;
	return (new_dentry);
}
