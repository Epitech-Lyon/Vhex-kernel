#include <kernel/fs/smemfs.h>
#include <kernel/util/kmem.h>
#include <string.h>

struct smemfs_USB3_inode *smemfs_USB3_alloc_inode(void)
{
	struct smemfs_USB3_inode *inode;

	// Try to alloc new inode
	inode = kmem_alloc(sizeof(struct smemfs_USB3_inode));
	if (inode != NULL)
		memset(inode, 0x00, sizeof(struct smemfs_USB3_inode));
	return (inode);
}
