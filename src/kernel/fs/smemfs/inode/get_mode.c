#include <kernel/fs/smemfs.h>
#include <kernel/fs/stat.h>

mode_t smemfs_get_mode(void *inode)
{
	extern struct smemfs_superblock_s smemfs_superblock;
	casio_smem_header_t *header;
	int inode_type;

	// Check error
	if (inode == NULL)
		return (-1);

	// Check root inode
	if (inode == smemfs_superblock.sector_table)
		return (__S_IFDIR | S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH | S_IWOTH);

	// Check inode validity
	header = inode;
	if (header->info != CASIO_SMEM_HEADER_INFO_EXIST)
		return (-1);

	// We can only check if the file is a
	// directory or not (Bfile limitation)
	if (header->type == CASIO_SMEM_HEADER_TYPE_DIRECTORY)
		inode_type = __S_IFDIR;
	else
		inode_type = __S_IFREG;

	// Return default permission and type
	return (inode_type | S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH | S_IWOTH);
}
