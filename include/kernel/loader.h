#ifndef __KERNEL_LOADER_H__
# define __KERNEL_LOADER_H__

#include <stddef.h>
#include <stdint.h>
#include <kernel/fs/file.h>
#include <kernel/process.h>
#include <kernel/util/elf.h>

// Function
extern int loader(struct process *process, const char *path);
extern int loader_get_header(FILE *file, Elf32_Ehdr *header);
extern int loader_load_image(struct process *process, FILE *file, Elf32_Ehdr *header);

#endif /*__KERNEL_LOADER_H__*/
