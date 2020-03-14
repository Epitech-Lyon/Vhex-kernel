#include <kernel/fs/vfs.h>
#include <kernel/memory.h>
#include <lib/string.h>

/* vfs_dentry_alloc() - Allocate new "empty" dentry */
struct dentry *vfs_dentry_alloc(const char *name, mode_t mode)
{
	struct dentry *node;

	// Try to create new dentry
	node = pm_alloc(sizeof(struct dentry));
	if (node == NULL)
		return (NULL);

	// Initialize dentry
	memset(node->name, 0x00, VFS_DENTRY_NAME_LENGHT);

	// Set the name if possible
	if (name != NULL)
		strncpy(node->name, name, VFS_DENTRY_NAME_LENGHT);

	// Set the mode
	node->mode = mode;

	// Set default value
	node->inode = NULL;
	node->parent = NULL;
	node->child = NULL;
	node->next = NULL;
	node->mnt.inode = NULL;
	node->mnt.file_op = NULL;
	node->mnt.inode_op = NULL;
	node->dentry_op.file_op = NULL;
	node->dentry_op.inode_op = NULL;
	return (node);
}
