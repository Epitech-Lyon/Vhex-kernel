#include <kernel/loader.h>
#include <kernel/fs/smem.h>
#include <kernel/fs/file.h>
#include <kernel/util.h>

void *loader(const char *path)
{
	FILE file;

	// TODO: use VFS !
	if (casio_smem_open(&file, path, O_RDONLY) != 0)
		return (NULL);

	kvram_clear();
	kvram_print(0, 0, "File found !!");
	kvram_print(0, 1, "inode = %p", file.abstract);
	kvram_print(0, 2, "path  = %s", path);
	kvram_display();
	while (1);

	// TODO: detect format !!
	//return (loader_elf(file));
	return (NULL);
}
