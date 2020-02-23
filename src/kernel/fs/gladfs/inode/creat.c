#include <kernel/fs/gladfs.h>
#include <kernel/fs/stat.h>
#include <kernel/util/atomic.h>

/* gladfs_creat() - Create new "empty" inode (atomically, sync) */
void *gladfs_creat(void *parent_inode, const char *file_name, mode_t mode)
{
	extern struct gladfs_superblock_s gladfs_superblock;
	struct gladfs_inode_s *new_inode;
	struct gladfs_inode_s *parent;

	// Start atomic operation
	atomic_start();

	// Create new inode
	new_inode = gladfs_superblock.super_op.alloc_inode(file_name, mode | __S_IFREG);
	if (new_inode == NULL)
	{
		atomic_stop();
		return (NULL);
	}

	// Update FHS
	parent = parent_inode;
	new_inode->parent = parent_inode;
	new_inode->next = parent->children;
	parent->children = new_inode;

	// Stop atomic operation
	atomic_stop();

	// Return inode
	return(new_inode);
}
