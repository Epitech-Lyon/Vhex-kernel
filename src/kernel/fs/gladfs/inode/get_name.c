#include <kernel/fs/gladfs.h>
#include <kernel/util/atomic.h>
#include <kernel/util/string.h>

/* gladfs_get_name() - Dump the name of a file (sync) */
int gladfs_get_name(void *inode, char *name, size_t count)
{
	// Check potential error
	if (inode == NULL || name == NULL)
		return (-1);

	// Check count
	if (count > GLADFS_INODE_NAME_LENGHT)
		count = GLADFS_INODE_NAME_LENGHT;

	// Start atomic operation
	atomic_start();

	// Dump name
	strncpy(name, ((struct gladfs_inode_s*)inode)->name, count);

	// Stop atomic operation
	atomic_stop();
	return (0);
}
