#include <kernel/loader.h>
#include <kernel/fs/vfs.h>
#include <kernel/fs/file.h>
#include <kernel/util.h>
#include <kernel/elf.h>

void *loader(const char *path, struct process *process)
{
	Elf32_Ehdr header;
	FILE file;

	// Check error.
	if (process == NULL)
	{
		kvram_clear();
		printk(0, 0, "loader: Fault error !");
		printk(0, 1, "path: %s$", path);
		printk(0, 2, "process: %p", process);
		kvram_display();
		DBG_WAIT;
		return (NULL);
	}

	// TODO: use VFS !
	if (vfs_open(&file, path, O_RDONLY) != 0)
	{
		kvram_clear();
		printk(0, 0, "loader: File open error !");
		printk(0, 1, "path: %s$", path);
		kvram_display();
		DBG_WAIT;
		return (NULL);
	}

	// Debug !
	kvram_clear();
	printk(0, 0, "File found !!");
	printk(0, 1, "inode = %p", file.private);
	printk(0, 2, "path  = %s", path);
	kvram_display();
	DBG_WAIT;

	//@NOTE: Only ELF supported !!!
	//TODO: USE VFS !!
	//
	// Get / Check ELF header
	if (loader_get_header(&file, &header) != 0)
	{
		kvram_clear();
		printk(0, 0, "loader: ELF file header error !");
		kvram_display();
		DBG_WAIT;
		return (NULL);
	}

	// Get / Check program validity
	void *entry = loader_load_image(&file, &header, process);
	if (entry == NULL)
	{
		kvram_clear();
		printk(0, 0, "loader: ELF file image error !");
		kvram_display();
		DBG_WAIT;
		return (NULL);
	}

	// Success !
	kvram_clear();
	printk(0, 0, "loader: file loaded sucees !!");
	printk(0, 1, "loader: Entry at %p", entry);
	kvram_display();
	DBG_WAIT;
	return (entry);
}
