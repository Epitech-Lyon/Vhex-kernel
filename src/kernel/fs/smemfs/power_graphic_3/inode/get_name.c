#include <kernel/fs/smemfs.h>
#include <kernel/util/atomic.h>
#include <string.h>

int smemfs_USB3_get_name(void *inode, char *name, size_t count)
{
	extern struct smemfs_USB3_superblock smemfs_USB3_superblock;
	struct smemfs_USB3_inode *smemfs_USB3_inode;

	// Check error
	if (inode == NULL)
		return (-1);

	// Get smemfs inode
	smemfs_USB3_inode = inode;

	// Get file name 
	atomic_start();

	// Check fake root inode
	if (inode == smemfs_USB3_superblock.fake_root_inode) {
		name[0] = '/';
		name[1] = '\0';
		atomic_stop();
		return (0);
	}

	strncpy(name, smemfs_USB3_inode->name, count);
	atomic_stop();

	// Return count
	// TODO: real count
	return (count);
}
