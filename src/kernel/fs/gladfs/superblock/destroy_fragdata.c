#include <kernel/fs/gladfs.h>
#include <kernel/memory.h>

int gladfs_destroy_fragdata(struct gladfs_fragment_data_s *fragment)
{
	// Check error
	if (fragment == NULL)
		return (-1);

	// Free'd allocated space
	pm_free(fragment);
	return (0);
}
