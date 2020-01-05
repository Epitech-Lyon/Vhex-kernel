#include <kernel/fs/gladfs.h>

mode_t gladfs_get_mode(void *inode)
{
	return (((struct gladfs_inode_s*)inode)->mode);
}
