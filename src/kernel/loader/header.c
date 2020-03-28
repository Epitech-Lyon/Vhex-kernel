#include <kernel/loader.h>
#include <kernel/fs/vfs.h>

/* loader_get_header() - get ELF header and check validity */
int loader_get_header(FILE *file, Elf32_Ehdr *header)
{
	// Read ELF header.
	if (vfs_pread(file, header, sizeof(Elf32_Ehdr), 0) != sizeof(Elf32_Ehdr))
		return (-1);

	// Check magic number
	if (header->e_ident[EI_MAG0] != ELFMAG0 ||
			header->e_ident[EI_MAG1] != ELFMAG1 ||
			header->e_ident[EI_MAG2] != ELFMAG2 ||
			header->e_ident[EI_MAG3] != ELFMAG3)
		return (-2);

	// Check class
	if (header->e_ident[EI_CLASS] != ELFCLASS32)
		return (-3);

	// Check data encoding
	if (header->e_ident[EI_DATA] != ELFDATA2MSB)
		return (-4);

	// Check ELF type.
	if (header->e_type != ET_DYN)
		return (-5);

	// Check ELF specifique instruction
	if (header->e_machine != EM_SH)
		return (-6);

	// Check ELF version
	if (header->e_version != EV_CURRENT)
		return (-7);
	return (0);
}
