#include <kernel/loader.h>
#include <kernel/fs/vfs.h>
#include <kernel/memory.h>
#include <lib/string.h>

/* loader_load_image() - Load the program into Virtual Memory */
int loader_load_image(struct process *process, FILE *file, Elf32_Ehdr *header)
{
	Elf32_Phdr program;
	void *paddress;
	uint16_t i;

	// Walk one time to get program size and
	// check ELF validity.
	i = -1;
	process->memory.program.size = 0;
	while (++i < header->e_phnum)
	{
		// Read programme header.
		vfs_lseek(file, header->e_phoff + (sizeof(Elf32_Phdr) * i), SEEK_SET);
		if (vfs_read(file, &program, sizeof(Elf32_Phdr)) != sizeof(Elf32_Phdr))
			return (-1);

		// Check programe type.
		if (program.p_type != PT_LOAD)
			return (-2);
		
		// Update program size
		process->memory.program.size = process->memory.program.size + program.p_memsz;
	}

	// Allocate programe space into
	// physical memory.
	process->memory.program.start = (void *)pm_alloc(process->memory.program.size);
	if (process->memory.program.start == 0x00000000)
		return (-3);

	// Now, load all program section into
	// physical memory.
	i = -1;
	while (++i < header->e_phnum)
	{
		// Read programme header.
		vfs_lseek(file, header->e_phoff + (sizeof(Elf32_Phdr) * i), SEEK_SET);
		vfs_read(file, &program, sizeof(Elf32_Phdr));

		// Generate physical address
		paddress = program.p_vaddr + process->memory.program.start;

		// p_filesz can be smaller than p_memsz so, we need to wipe the area
		// before the dump.
		memset(paddress, 0, program.p_memsz);

		// Dump the program. 
		vfs_lseek(file, program.p_offset, SEEK_SET);
		vfs_read(file, paddress, program.p_filesz);
	}

	// Generate program entry address
	process->context.spc = (uint32_t)(header->e_entry + (uint32_t)process->memory.program.start);
	return ((process->context.spc == 0x00000000));
}
