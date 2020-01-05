#include <kernel/fs/gladfs.h>
#include <kernel/memory.h>

int gladfs_destroy_inode(struct gladfs_inode_s *inode)
{
	struct gladfs_fragment_data_s *fragdata;
	struct gladfs_fragment_data_s *next;

	// Check error.
	if (inode == NULL)
		return (-1);

	// Free fragmented data
	fragdata = inode->fragdata;
	while (fragdata == NULL)
	{
		// Get next fragement
		next = (void *)fragdata->next;
		pm_free(fragdata);

		// Update current fragment
		fragdata = next;
	}

	// Free inode
	pm_free(inode);
	return (0);
}
