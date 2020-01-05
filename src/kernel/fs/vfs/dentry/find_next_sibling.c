#include <kernel/fs/vfs.h>
#include <kernel/memory.h>
#include <kernel/util.h>

struct dentry *vfs_dentry_find_next_sibling(struct dentry *dentry)
{
	// Check error.
	if (dentry == NULL)
		return (NULL);

	// Try to find next sibling into the VFS cache
	if (dentry->next != NULL)
		return (dentry->next);

	// Check internal primitives
	if (dentry->dentry_op.inode_op == NULL ||
		dentry->dentry_op.inode_op->find_next_sibling == NULL ||
		dentry->dentry_op.inode_op->get_mode == NULL ||
		dentry->dentry_op.inode_op->get_name == NULL)
		return (NULL);

	// Try to get next inode
	void *inode = dentry->dentry_op.inode_op->find_next_sibling(dentry->inode);
	if (inode == NULL)
		return (NULL);

	// Try to create new dentry
	struct dentry *new_dentry = pm_alloc(sizeof(struct dentry));
	if (new_dentry == NULL)
		return (NULL);

	// Initialize new dentry
	dentry->dentry_op.inode_op->get_name(inode, new_dentry->name, VFS_DENTRY_NAME_LENGHT);
	new_dentry->mode = dentry->dentry_op.inode_op->get_mode(inode);
	new_dentry->inode = inode;
	new_dentry->parent = dentry;
	new_dentry->child = NULL;
	new_dentry->next = dentry->next;
	new_dentry->dentry_op.file_op = dentry->dentry_op.file_op;
	new_dentry->dentry_op.inode_op = dentry->dentry_op.inode_op;
	new_dentry->mnt.inode = NULL;
	new_dentry->mnt.file_op = NULL;
	new_dentry->mnt.inode_op = NULL;

	// Update VFS cache en return
	dentry->next = new_dentry;
	return (new_dentry);

}
