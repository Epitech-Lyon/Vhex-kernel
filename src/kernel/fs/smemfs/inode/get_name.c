#include <kernel/fs/smemfs.h>
#include <kernel/util/atomic.h>

/* smemfs_get_name() - Dump the name of a file (sync) */
int smemfs_get_name(void *inode, char *buf, size_t count)
{
	extern struct smemfs_superblock_s smemfs_superblock;
	struct casio_smem_header_s *header;

	// Check error
	if (inode == NULL)
		return (-1);

	// Start atomic operation
	atomic_start();

	// Check root inode
	if (inode == smemfs_superblock.sector_table)
	{
		buf[0] = '/';
		buf[1] = '\0';
		atomic_stop();
		return (0);
	}

	// Check inode validity
	header = inode;
	if (header->info != CASIO_SMEM_HEADER_INFO_EXIST)
	{
		atomic_stop();
		return (-1);
	}

	// Get "real" name size
	count = count - 1;
	if (count > CASIO_SMEM_NAME_LENGHT)
		count = CASIO_SMEM_NAME_LENGHT;

	// Dump name
	// @note: The name is stored into Shift-JIS format
	int i = -1;
	while (++i < (int)count &&
			header->name[i] != 0x0000 &&
			header->name[i] != 0xffff)
		buf[i] = header->name[i];
	buf[i] = '\0';

	// Start atomic operation
	atomic_stop();
	return (0);
}
