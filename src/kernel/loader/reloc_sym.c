#include <kernel/loader.h>
#include <kernel/devices/earlyterm.h>
#include <kernel/util/kmem.h>
#include <kernel/memory.h>
#include <kernel/fs/vfs.h>

static char *get_shstrtab(FILE *file, Elf32_Ehdr *header)
{
	Elf32_Shdr shdr;
	char *shstrtab;
	off_t offset;

	// Get sections string header tables
	offset = header->e_shoff + (header->e_shstrndx * header->e_shentsize);
	earlyterm_write("pos = %#x\n", offset);
	if (vfs_pread(file, &shdr, header->e_shentsize, offset) != header->e_shentsize)
	{
		earlyterm_write("relo_sym: shdr size\n");
		return (NULL);
	}

	// Allocate dump area
	earlyterm_write("Try to alloc strtab (%do)\n", shdr.sh_size);
	shstrtab = (char*)kmem_alloc(shdr.sh_size);
	if (shstrtab == NULL)
	{
		earlyterm_write("relo_sym:mem (%d)\n", shdr.sh_size);
		return (NULL);
	}

	// Get string tables
	earlyterm_write("pos = %#x\n", shdr.sh_offset);
	if (vfs_pread(file, shstrtab, shdr.sh_size, shdr.sh_offset) != (ssize_t)shdr.sh_size)
	{
		earlyterm_write("relo_sym: shstrtab size error\n");
		kmem_free(shstrtab);
		return (NULL);
	}
	return (shstrtab);
}

static int reloc_section(struct process *process, FILE *file, Elf32_Shdr *shdr)
{
	Elf32_Rela rela;
	uint32_t *prog;
	off_t offset;

	// List symbols
	prog = (void*)process->memory.program.start;
	for (uint32_t i = 0 ; i < shdr->sh_size / shdr->sh_entsize ; ++i)
	{
		// Get relocatable entry
		offset = shdr->sh_offset + (i * shdr->sh_entsize);
		earlyterm_write("rela\npos = %#x\n", offset);
		if (vfs_pread(file, &rela, shdr->sh_entsize, offset) != (ssize_t)shdr->sh_entsize)
		{
			earlyterm_write("relo_sym: reloc section size error\n");
			return (-1);
		}

		// Relocalise symbols
		//earlyterm_write("* [%d] %p, %#x\n", i, rela.r_offset, rela.r_addend);
		prog[rela.r_offset >> 2] += (uint32_t)process->memory.program.start;
		//DBG_WAIT;
	}
	return (0);
}

int loader_reloc_sym(struct process *process, FILE *file, Elf32_Ehdr *header)
{
	Elf32_Shdr shdr;
	char *shstrtab;
	off_t offset;

	// DEBUG
	earlyterm_write("e_shoff    = %d\n", header->e_shoff);
	earlyterm_write("e_shnum    = %d\n", header->e_shnum);
	earlyterm_write("e_shstrndx = %d\n", header->e_shstrndx);
	//DBG_WAIT;

	// Get the shstrtab
	shstrtab = get_shstrtab(file, header);
	if (shstrtab == NULL)
		return (-1);

	// List header tables
	for (int i = 1 ; i < header->e_shnum ; ++i)
	{
		// Get next section header
		offset = header->e_shoff + (i * header->e_shentsize);
		if (vfs_pread(file, &shdr, header->e_shentsize, offset) != header->e_shentsize)
		{
			earlyterm_write("loader_reloc_sym: section header\n");
			earlyterm_write("loader_reloc_sym: shoff = %#x\n", header->e_shoff);
			earlyterm_write("loader_reloc_sym: shnum = %#x\n", header->e_shnum);
			earlyterm_write("loader_reloc_sym: off = %#x\n", offset);
			return (-2);
		}

		// Check relocatable sections
		if (shdr.sh_type != SHT_RELA)
			continue;

		// Diplay section name
		earlyterm_write("[%d] %s - %d\n", i, &shstrtab[shdr.sh_name], shdr.sh_size / shdr.sh_entsize);
		if (reloc_section(process, file, &shdr) != 0)
			return (-3);
	}
	kmem_free(shstrtab);
	return (0);
}
