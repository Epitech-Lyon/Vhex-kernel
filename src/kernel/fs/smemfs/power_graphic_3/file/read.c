#include <kernel/fs/smemfs.h>
#include <kernel/driver.h>
#include <kernel/devices/earlyterm.h>
#include <kernel/util/casio.h>
#include <string.h>

static void generate_absolute_path(uint16_t *pathname, struct smemfs_USB3_inode *inode, int *pos)
{
	// Root inode
	if (inode == NULL) {
		memcpy(pathname, u"\\\\fls0", 12);
		*pos = 6;
		return;
	}

	// Check parent inode
	generate_absolute_path(pathname, inode->parent, pos);

	// Insert file name
	pathname[(*pos)++] = '\\'; 
	for (int i = 0 ; inode->name[i] != '\0' ; )
	{
		pathname[*pos] = inode->name[i];
		*pos = *pos + 1;
		i = i + 1;
	}
	pathname[*pos] = '\0';
}

static void display_test(uint16_t *buffer)
{
	for (int i = 0 ; buffer[i] != 0x0000 ; ++i)
		earlyterm_write("%c", buffer[i] & 0x00ff);
	earlyterm_write("\n");
	DBG_WAIT;
}

ssize_t	smemfs_USB3_read(void *inode, void *buf, size_t count, off_t pos)
{
	struct smemfs_USB3_inode *smemfs_USB3_inode;
	uint16_t pathname[64];
	ssize_t read;
	int handle;

	// Check obvious error
	if (inode == NULL)
		return (-1);

	// Get inode
	smemfs_USB3_inode = inode;
	generate_absolute_path(pathname, inode, &handle);

	// We should use internal Casio's `Bfile_*` syscall
	// to dump SMEM content
	drivers_uninstall(0);

	//DEBUG
	display_test(pathname);

	// Open the file
	read = -1;
	handle = casio_Bfile_OpenFile(pathname, _OPENMODE_READ);
	if (handle >= 0)
	{
		read = casio_Bfile_ReadFile(handle, buf, count, pos);
		casio_Bfile_CloseFile(handle);
	}

	// Restore all drivers
	drivers_install(0);

	// Return the number of readed bytes
	return (read);
}
