#include <kernel/fs/gladfs.h>
#include <kernel/memory.h>
#include <kernel/util/atomic.h>

/* gladfs_destroy_fragdata() - Free'd allocated fragmented data (sync) */
/* @note: *WARNING* no verification will be done, so do not use this primitive */
int gladfs_destroy_fragdata(struct gladfs_fragment_data_s *fragment)
{
	// Check error
	if (fragment == NULL)
		return (-1);

	// Start atomic operation
	atomic_start();

	// Free'd allocated space
	pm_free(fragment);

	// Stop atomic operation
	atomic_stop();
	return (0);
}
