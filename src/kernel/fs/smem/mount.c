#include <kernel/fs/smem.h>

// Internal symbols
casio_smem_header_t *casio_smem_header_table;
casio_smem_block_t  *casio_smem_block_table;
uint16_t current_parent_id;

/* casio_smem_mount() - Get Block and Preheader Table addresses */
int casio_smem_mount(void)
{
	// Casio SMEM block table start always at 0xa0270000
	casio_smem_block_table = (void *)0xa0270000;
	//TODO: check block table intergrity.

	// Casio SMEM preheader table start always at the end of the block table.
	// Normaly start at 0xa0270320 but not always (?)
	int i = -1;
	while (casio_smem_block_table[++i].magic_start == CASIO_SMEM_BLOCK_ENTRY_MAGIC);
	casio_smem_header_table = (void *)&casio_smem_block_table[i];
	
	//TODO: check header intergrity !
	
	current_parent_id = CASIO_SMEM_ROOT_ID;
	return (0);
}
