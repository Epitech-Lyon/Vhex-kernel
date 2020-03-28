#include <kernel/loader.h>
#include <kernel/fs/vfs.h>
#include <kernel/memory.h>
#include <lib/string.h>

/* loader_load_image() - Load the program into Virtual Memory */
int loader_load_image(struct process *process, FILE *file, Elf32_Ehdr *header)
{
	Elf32_Phdr program;
	void *paddress;
	off_t offset;
	uint16_t i;

	// Walk one time to get program size and
	// check ELF validity.
	i = -1;
	process->memory.program.size = 0;
	while (++i < header->e_phnum)
	{
		// Read programme header.
		offset = header->e_phoff + (i * sizeof(Elf32_Phdr));
		if (vfs_pread(file, &program, sizeof(Elf32_Phdr), offset) != sizeof(Elf32_Phdr))
			return (-1);

		// Check programe type.
		if (program.p_type != PT_LOAD)
			return (-2);
		
		// Update program size
		process->memory.program.size = process->memory.program.size + program.p_memsz;
	}

	// Allocate programe space into physical memory.
	// @note: we use page allocator to avoid kernel
	// heap grow up
	process->memory.program.start = pm_pages_alloc(PM_SIZE_TO_PAGES(process->memory.program.size));
	if (process->memory.program.start == NULL)
		return (-3);

	// Now, load all program section into
	// physical memory.
	i = -1;
	while (++i < header->e_phnum)
	{
		// Read programme header.
		offset = header->e_phoff + (i * sizeof(Elf32_Phdr));
		vfs_pread(file, &program, sizeof(Elf32_Phdr), offset);

		// Generate physical address
		paddress = program.p_vaddr + process->memory.program.start;

		// p_filesz can be smaller than p_memsz so, we need to wipe the area
		// before the dump.
		memset(paddress, 0, program.p_memsz);

		// Dump the program. 
		vfs_pread(file, paddress, program.p_filesz, program.p_offset);
	}

	// Generate program entry address
	process->context.spc = (uint32_t)(header->e_entry + (uint32_t)process->memory.program.start);
	return ((process->context.spc == 0x00000000));
}
