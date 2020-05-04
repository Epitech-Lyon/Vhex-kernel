#include <kernel/fs/smemfs.h>
#include <kernel/util/atomic.h>
#include <kernel/util/casio.h>
#include <kernel/devices/earlyterm.h>
#include <kernel/driver.h>
#include <string.h>

static size_t wide_char_convert(char *pathname, uint16_t *pathname_wc)
{
	size_t i;

	i = -1;
	while (pathname_wc[++i] != 0x0000 && pathname_wc[i] != 0xffff)
		pathname[i] = pathname_wc[i] & 0x00ff;
	pathname[i] = '\0';
	return (i);
}

/*static void display_test(uint16_t *buffer)
{
	for (int i = 0 ; buffer[i] != 0x0000 ; ++i)
		earlyterm_write("%c", buffer[i] & 0x00ff);
	earlyterm_write("\n");
}*/

// @note: send buffer to avoid recursif buffer definition
static void dump_smem_level(struct smemfs_USB3_inode *parent, 
		struct smemfs_USB3_inode **sibling, uint16_t *buffer)
{
	struct casio_file_info file_info;
	struct smemfs_USB3_inode *inode;
	int handle;
	int i;

	// Generate search path
	i = 7;
	memcpy(buffer, u"\\\\fls0\\", 14);
	if (parent != NULL) {
		for (int j = 0 ; parent->name[j] != '\0' ; ) {
			buffer[i] = (uint16_t)(parent->name[j]);
			i = i + 1;
			j = j + 1;
		}
		buffer[i++] = '\\';
	}
	buffer[i + 0] = '*';
	buffer[i + 1] = 0x0000;

	//DEBUG
	//display_test(buffer);

	// Find the first file
	// @note: the search buffer and the buffer which will content
	// the file name is the same. But it's not used at the same time
	// so we can use this tricky way to save some stack
	if (casio_Bfile_FindFirst(buffer, &handle, buffer, &file_info) != 0)
		return;

	// Get all inode stored in this level
	i = 0;
	do {
		// Try to alloc new inode
		// TODO: return error code !
		*sibling = smemfs_USB3_alloc_inode();
		if (*sibling == NULL)
			break;

		// Get first inide
		if (i == 0)
			inode = *sibling;
		i = 1;

		// Convert wide char into char
		wide_char_convert((*sibling)->name, buffer);

		// Dump file informations
		(*sibling)->type = file_info.type;
		(*sibling)->fsize = file_info.size.file;
		(*sibling)->dsize = file_info.size.data;

		// Link node and get next sibling
		(*sibling)->parent = parent;
		sibling = &(*sibling)->sibling;

	} while (casio_Bfile_FindNext(handle, buffer, &file_info) == 0);

	// Close casio BfileFind* handle
	casio_Bfile_FindClose(handle);

	// Now let's check all file to find directories
	while (inode != NULL)
	{
		// Check directory type
		if (inode->type == DT_DIRECTORY)
			dump_smem_level(inode, &inode->child, buffer);

		// Get next inode
		inode = inode->sibling;
	}
}

static void proto_ls(struct smemfs_USB3_inode *inode, int level)
{
	if (inode == NULL)
		return;
	for (int i = 0 ; i < level ; ++i)
		earlyterm_write("  ");
	earlyterm_write("%s\n", inode->name);
	if (inode->child != NULL)
		proto_ls(inode->child, level + 1);
	proto_ls(inode->sibling, level);
}

/*
** smemfs_USB3_mount() - Mount the file system (sync)
** @note:
** We don't known how the file system work, so we should use
** Casio's "Bfile_*" sycalls to dump all internal informations
** to avoid OS switch (Vhex -> Casio -> Vhex)
*/
void *smemfs_USB3_mount(void)
{
	extern struct smemfs_USB3_superblock smemfs_USB3_superblock;
	uint16_t buffer[64];
	void *root_inode;

	// Get current root inode
	atomic_start();
	root_inode = smemfs_USB3_superblock.root_inode;
	atomic_stop();

	// Check useless mount
	if (root_inode != NULL)
		return (root_inode);

	// We should use internal Casio's `Bfile_*` syscall
	// to dump SMEM content
	drivers_uninstall(0);

	// Generate fake root inode
	smemfs_USB3_superblock.fake_root_inode = (void*)0xdeadbeff;

	// Dump SMEM files organisation
	smemfs_USB3_superblock.root_inode = NULL;
	dump_smem_level(smemfs_USB3_superblock.root_inode,
			&smemfs_USB3_superblock.root_inode, buffer);

	// Get the "fake" root inode
	root_inode = smemfs_USB3_superblock.fake_root_inode;

	//DEBUG
	//proto_ls(smemfs_USB3_superblock.root_inode, 0);
	//earlyterm_write("TAMER !\n");
	//while (1);

	// Restore all drivers
	drivers_install(0);

	// Return the sector table to simulate the root inode.
	return (root_inode);
}
