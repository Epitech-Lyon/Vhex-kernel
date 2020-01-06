#include <kernel/fs/vfs.h>
#include <kernel/memory.h>

void vfs_dentry_free(struct dentry *dentry)
{
	// Check error
	if (dentry == NULL)
		return;

	// Free'd allocated space
	pm_free(dentry);
}
