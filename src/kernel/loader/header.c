#include <kernel/loader.h>
#include <kernel/fs/vfs.h>
#include <kernel/util.h>
#include <kernel/elf.h>

/* loader_get_header() - get ELF header and check validity */
int loader_get_header(FILE *file, Elf32_Ehdr *header)
{
	// Read ELF header.
	if (vfs_read(file, header, sizeof(Elf32_Ehdr)) != sizeof(Elf32_Ehdr))
		return (-1);

	// Check magic number
	if (header->e_ident[EI_MAG0] != 0x7f ||
			header->e_ident[EI_MAG1] != 'E' ||
			header->e_ident[EI_MAG2] != 'L' ||
			header->e_ident[EI_MAG3] != 'F')
		return (-2);

	// Check class
	if (header->e_ident[EI_CLASS] != 1)
		return (-3);

	//TODO: Check OSABI (?)

	// Check ELF type.
	if (header->e_type != ET_EXEC)
		return (-4);

	// Check ELF specifique instruction
	if (header->e_machine != 0x2A)
		return (-5);

	// Check ELF version
	if (header->e_version != 0x01)
		return (-6);
	return (0);
}
