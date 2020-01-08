#include <kernel/fs/gladfs.h>

/* gladfs_get_mode() - Dump permission and type of a file (async) */
mode_t gladfs_get_mode(void *inode)
{
	return (((struct gladfs_inode_s*)inode)->mode);
}
