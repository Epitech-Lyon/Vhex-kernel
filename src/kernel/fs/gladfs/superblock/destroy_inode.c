#include <kernel/fs/gladfs.h>
#include <kernel/util/kmem.h>
#include <kernel/util/atomic.h>

/* gladfs_destroy_inode() - Free'd allocated inode (sync) */
/* @note: *WARNING* no verification will be done, so do not use this primitive */
int gladfs_destroy_inode(struct gladfs_inode_s *inode)
{
	struct gladfs_fragment_data_s *fragdata;
	struct gladfs_fragment_data_s *next;

	// Check error.
	if (inode == NULL)
		return (-1);

	// Start atomic operations
	atomic_start();

	// Free fragmented data
	fragdata = inode->fragdata;
	while (fragdata == NULL)
	{
		// Get next fragement
		next = (void *)fragdata->next;
		gladfs_destroy_fragdata(fragdata);

		// Update current fragment
		fragdata = next;
	}

	// Free inode
	kmem_free(inode);

	// Stop atomic operations
	atomic_stop();
	return (0);
}
