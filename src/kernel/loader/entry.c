#include <kernel/loader.h>
#include <kernel/fs/vfs.h>
#include <kernel/fs/file.h>
#include <kernel/util/elf.h>
#include <kernel/devices/earlyterm.h>

//
// TODO: write doc
//
int loader(struct process *process, const char *path)
{
	Elf32_Ehdr header;
	FILE file;

	// Check error and try to open the file.
	if (process == NULL || vfs_open(&file, path, O_RDONLY) != 0)
	{
		earlyterm_write("loader: Fault error !");
		earlyterm_write("* path: %s$", path);
		earlyterm_write("* process: %p", process);
		DBG_WAIT;
		return (-1);
	}

	//@NOTE: Only ELF supported !!!
	// Get / Check ELF header
	// FIXME: return the file type (elf, g1a, bin, ...)
	if (loader_get_header(&file, &header) != 0)
	{
		earlyterm_write("loader: ELF file header error !");
		DBG_WAIT;
		return (-2);
	}

	// Get / Check program validity
	if (loader_load_image(process, &file, &header) != 0)
	{
		earlyterm_write("loader: ELF file image error !");
		DBG_WAIT;
		return (-3);
	}
	return (0);
}
