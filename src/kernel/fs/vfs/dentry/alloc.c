#include <kernel/fs/vfs.h>
#include <kernel/memory.h>
#include <kernel/util.h>

struct dentry *vfs_dentry_alloc(const char *name, mode_t mode)
{
	struct dentry *node;

	// Try to create new dentry
	node = pm_alloc(sizeof(struct dentry));
	if (node == NULL)
		return (NULL);

	// Initialize dentry
	memset(node->name, 0x00, VFS_DENTRY_NAME_LENGHT);
	strncpy(node->name, name, VFS_DENTRY_NAME_LENGHT);
	node->inode = NULL;
	node->mode = mode;
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
