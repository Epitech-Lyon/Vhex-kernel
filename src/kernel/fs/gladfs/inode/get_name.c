#include <kernel/fs/gladfs.h>
#include <kernel/util.h>

int gladfs_get_name(void *inode, char *name, size_t count)
{
	// Check potential error
	if (inode == NULL || name == NULL)
		return (-1);

	// Check count
	if (count > GLADFS_INODE_NAME_LENGHT)
		count = GLADFS_INODE_NAME_LENGHT;

	// Dump name
	strncpy(name, ((struct gladfs_inode_s*)inode)->name, count);
	return (0);
}
