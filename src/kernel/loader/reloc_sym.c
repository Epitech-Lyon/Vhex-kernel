#include <kernel/loader.h>
#include <kernel/devices/earlyterm.h>
#include <kernel/memory.h>
#include <kernel/fs/vfs.h>

static char *get_shstrtab(FILE *file, Elf32_Ehdr *header)
{
	Elf32_Shdr shdr;
	char *shstrtab;

	// Get sections string header tables
	earlyterm_write(
			"off_t = %#x\noff_t = %#x\n",
			header->e_shoff + (header->e_shstrndx * sizeof(Elf32_Shdr)),
			vfs_lseek(file, header->e_shoff + (header->e_shstrndx * sizeof(Elf32_Shdr)), SEEK_SET)
	);;
	if (vfs_read(file, &shdr, sizeof(Elf32_Shdr)) != sizeof(Elf32_Shdr))
	{
		earlyterm_write("relo_sym: section header table error\n");
		return (NULL);
	}

	// Allocate dump area
	shstrtab = (char*)pm_alloc(shdr.sh_size);
	if (shstrtab == NULL)
	{
		earlyterm_write("relo_sym: mem error (%d)\n", shdr.sh_size);
		return (NULL);
	}

	// Get string tables
	vfs_lseek(file, shdr.sh_offset, SEEK_SET);
	if (vfs_read(file, shstrtab, shdr.sh_size) != (ssize_t)shdr.sh_size)
	{
		earlyterm_write("relo_sym: shstrtab size error\n");
		pm_free(shstrtab);
		return (NULL);
	}
	return (shstrtab);
}

static int reloc_section(struct process *process, FILE *file, Elf32_Shdr *shdr)
{
	Elf32_Rela rela;
	uint32_t *prog;

	// List symbols
	prog = (void*)process->memory.program.start;
	for (uint32_t i = 0 ; i < shdr->sh_size / shdr->sh_entsize ; ++i)
	{
		// Get relocatable entry
		vfs_lseek(file, shdr->sh_offset + (i * sizeof(Elf32_Rela)), SEEK_SET);
		if (vfs_read(file, &rela, sizeof(Elf32_Rela)) != sizeof(Elf32_Rela))
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

	//TODO
	(void)process;
	
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
		vfs_lseek(file, header->e_shoff + (i * sizeof(Elf32_Shdr)), SEEK_SET);
		if (vfs_read(file, &shdr, sizeof(Elf32_Shdr)) != sizeof(Elf32_Shdr))
		{
			earlyterm_write("loader_reloc_sym: section header\n");
			earlyterm_write("loader_reloc_sym: shoff = %#x\n", header->e_shoff);
			earlyterm_write("loader_reloc_sym: shnum = %#x\n", header->e_shnum);
			earlyterm_write("loader_reloc_sym: off = %#x\n", header->e_shoff + (i * sizeof(Elf32_Shdr)));
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
	pm_free(shstrtab);
	//earlyterm_write("start = %p\n", process->memory.program.start);
	DBG_WAIT;
	DBG_WAIT;
	//DBG_WAIT;
	return (0);
}
