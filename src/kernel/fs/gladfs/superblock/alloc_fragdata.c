#include <kernel/fs/gladfs.h>
#include <kernel/memory.h>
#include <kernel/atomic.h>

/* gladfs_alloc_fragdata() - Superblock primitive to alloc "empty" fragment data block (sync) */
int gladfs_alloc_fragdata(struct gladfs_fragment_data_s **parent, int nb_block)
{
	extern struct gladfs_superblock_s gladfs_superblock;

	// Check error
	if (parent == NULL)
		return (-1);

	// Start atomic opeation
	atomic_start();

	// Try to alloc block
	*parent = pm_alloc(gladfs_superblock.block_size * nb_block);
	if (*parent == NULL)
	{
		atomic_stop();
		return (-1);
	}

	// Fill default value.
	(*parent)->next = 0x00000000;
	(*parent)->data_size = gladfs_superblock.block_size * nb_block;
	(*parent)->data_used = 0x00000000;

	// Stop atomic operation
	atomic_stop();
	return (0);
}
