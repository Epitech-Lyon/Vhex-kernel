#include <kernel/fs/vfs.h>

// Internal cache for registered FS
struct file_system_type *fs_list = NULL;

// vfs_register_filesystem() -  Register a new FS into kernel supported FS list
int vfs_register_filesystem(struct file_system_type *new)
{
	// Check error.
	// TODO: check FS validity
	if (new == NULL)
		return (-1);

	// Register new FS.
	new->next = fs_list;
	fs_list = new;
	return (0);
}
