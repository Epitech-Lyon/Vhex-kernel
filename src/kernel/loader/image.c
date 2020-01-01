#include <kernel/loader.h>
#include <kernel/fs/smem.h>
#include <kernel/memory.h>
#include <kernel/util.h>
#include <kernel/elf.h>

/* loader_load_image() - Load the program into Virtual Memory */
void *loader_load_image(FILE *file, Elf32_Ehdr *header, process_t *process)
{
	Elf32_Phdr program;
	uint32_t paddress;
	uint16_t i;

	// Walk one time to get program size and
	// check ELF validity.
	i = -1;
	process->memory.program.size = 0;
	while (++i < header->e_phnum)
	{
		// Read programme header.
		casio_smem_lseek(file, header->e_phoff + (sizeof(Elf32_Phdr) * i), SEEK_SET);
		if (casio_smem_read(file, &program, sizeof(Elf32_Phdr)) != sizeof(Elf32_Phdr))
			return (NULL);

		// Check programe type.
		if (program.p_type != PT_LOAD)
			return (NULL);
		
		// Update program size
		process->memory.program.size = process->memory.program.size + program.p_memsz;
	}

	// Allocate programe space into
	// physical memory.
	process->memory.program.start = (uint32_t)pm_alloc(process->memory.program.size);
	if (process->memory.program.start == 0x00000000)
		return (NULL);

	// Now, load all program section into
	// physical memory.
	i = -1;
	while (++i < header->e_phnum)
	{
		// Read programme header.
		casio_smem_lseek(file, header->e_phoff + (sizeof(Elf32_Phdr) * i), SEEK_SET);
		casio_smem_read(file, &program, sizeof(Elf32_Phdr));

		// Generate physical address
		paddress = program.p_vaddr + process->memory.program.start;

		// p_filesz can be smaller than p_memsz so, we need to wipe the area
		// before the dump.
		memset((void *)paddress, 0, program.p_memsz);

		// Dump the program. 
		casio_smem_lseek(file, program.p_offset, SEEK_SET);
		casio_smem_read(file, (void *)paddress, program.p_filesz);
	}

	// Generate program entry address
	return ((void*)(header->e_entry + process->memory.program.start));
}
