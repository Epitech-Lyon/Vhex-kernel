#include <kernel/loader.h>
#include <kernel/fs/vfs.h>
#include <kernel/fs/file.h>
#include <kernel/devices/earlyterm.h>
#include <fcntl.h>
#include <elf.h>

//
// TODO: write doc
//
int loader(struct process *process, const char *path)
{
	Elf32_Ehdr header;
	FILE file;
	int err;

	// Check error and try to open the file.
	if (process == NULL || vfs_open(&file, path, O_RDONLY) != 0)
	{
		earlyterm_write("loader: Fault error !\n");
		earlyterm_write("* path: %s$\n", path);
		earlyterm_write("* process: %p\n", process);
		DBG_WAIT;
		return (-1);
	}

	//@NOTE: Only ELF supported !!!
	// Get / Check ELF header
	// FIXME: return the file type (elf, g1a, bin, ...)
	err = loader_get_header(&file, &header);
	if (err != 0)
	{
		earlyterm_write("loader: ELF file header error ! (%d)\n", err);
		DBG_WAIT;
		return (-2);
	}

	// Get / Check program validity
	err = loader_load_image(process, &file, &header);
	if (err != 0)
	{
		earlyterm_write("loader: ELF file image error ! (%d)\n", err);
		DBG_WAIT;
		return (-3);
	}

	// Relocalise sections / symbols
	err = loader_reloc_sym(process, &file, &header);
	if (err != 0)
	{
		earlyterm_write("loader: ELF relo error ! (%d)\n", err);
		DBG_WAIT;
		return (-4);
	}

	// Close file
	vfs_close(&file);
	return (0);
}
