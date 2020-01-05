#include <kernel/fs/gladfs.h>
#include <kernel/memory.h>
#include <kernel/util.h>

struct gladfs_inode_s *gladfs_alloc_inode(const char *name, mode_t mode)
{
	struct gladfs_inode_s *inode;

	// alloc memory
	inode = pm_alloc(sizeof(struct gladfs_inode_s));
	if (inode == NULL)
		return (NULL);

	// Fill inode.
	memset(inode, 0x00, sizeof(struct gladfs_inode_s));
	strncpy(inode->name, name, GLADFS_INODE_NAME_LENGHT);
	inode->mode		= mode;
	inode->parent		= NULL;
	inode->children		= NULL;
	inode->next		= NULL;
	inode->size		= 0;
	inode->fragnumber	= 0;
	inode->fragdata		= NULL;
	return (inode);
}
