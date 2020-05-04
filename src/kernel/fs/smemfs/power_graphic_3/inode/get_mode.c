#include <kernel/fs/smemfs.h>
#include <kernel/fs/stat.h>
#include <kernel/util/atomic.h>
#include <kernel/util/casio.h>

mode_t smemfs_USB3_get_mode(void *inode)
{
	extern struct smemfs_USB3_superblock smemfs_USB3_superblock;
	struct smemfs_USB3_inode *smemfs_USB3_inode;
	mode_t inode_type;

	// Check error
	if (inode == NULL)
		return (-1);

	// Get smemfs inode
	smemfs_USB3_inode = inode;

	// Get file name 
	atomic_start();

	// Check fake root
	if (inode == smemfs_USB3_superblock.fake_root_inode) {
		atomic_stop();
		return (__S_IFDIR | S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH | S_IWOTH);
	}

	// We can only check if the file is a
	// directory or not (Bfile limitation)
	// TODO ?
	inode_type = __S_IFREG;
	if (smemfs_USB3_inode->type == DT_DIRECTORY)
		inode_type = __S_IFDIR;

	atomic_stop();

	// Return default permission and type
	return (inode_type | S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH | S_IWOTH);
}
