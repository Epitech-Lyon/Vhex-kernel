#ifndef __ELF_H__
# define __ELF_H__	1

#include <stddef.h>
#include <stdint.h>

typedef uint16_t Elf32_Half;		/* Type for a 16-bit quantity. */
typedef uint32_t Elf32_Word;		/* Types for signed and unsigned 32-bit quantities. */
typedef	int32_t  Elf32_Sword;		/* Types for signed and unsigned 32-bit quantities. */
typedef uint32_t Elf32_Addr;		/* Type of addresses. */
typedef uint32_t Elf32_Off;		/* Type of file offsets. */
typedef uint16_t Elf32_Section;		/* Type for section indices, which are 16-bit quantities. */
typedef Elf32_Half Elf32_Versym;	/* Type for version symbol information. */


/* The ELF file header.  This appears at the start of every ELF file.  */
#define EI_NIDENT (16)

typedef struct
{
	uint8_t		e_ident[EI_NIDENT];	/* Magic number and other info */
	Elf32_Half	e_type;			/* Object file type */
	Elf32_Half	e_machine;		/* Architecture */
	Elf32_Word	e_version;		/* Object file version */
	Elf32_Addr	e_entry;		/* Entry point virtual address */
	Elf32_Off	e_phoff;		/* Program header table file offset */
	Elf32_Off	e_shoff;		/* Section header table file offset */
	Elf32_Word	e_flags;		/* Processor-specific flags */
	Elf32_Half	e_ehsize;		/* ELF header size in bytes */
	Elf32_Half	e_phentsize;		/* Program header table entry size */
	Elf32_Half	e_phnum;		/* Program header table entry count */
	Elf32_Half	e_shentsize;		/* Section header table entry size */
	Elf32_Half	e_shnum;		/* Section header table entry count */
	Elf32_Half	e_shstrndx;		/* Section header string table index */
} Elf32_Ehdr;


/* Magic number macros */
#define EI_MAG0		0		/* File identification byte 0 index */
#define ELFMAG0		0x7f		/* Magic number byte 0 */
#define EI_MAG1		1		/* File identification byte 1 index */
#define ELFMAG1		'E'		/* Magic number byte 1 */
#define EI_MAG2		2		/* File identification byte 2 index */
#define ELFMAG2		'L'		/* Magic number byte 2 */
#define EI_MAG3		3		/* File identification byte 3 index */
#define ELFMAG3		'F'		/* Magic number byte 3 */

/* Class macros */
#define EI_CLASS	4		/* File class byte index */
#define ELFCLASS32	1		/* 32-bit objects */

/* Data encoding macros */
#define EI_DATA		5		/* Data encoding byte index */
#define ELFDATA2LSB	1		/* 2's complement, little endian */
#define ELFDATA2MSB	2		/* 2's complement, big endian */

/* ABI info macros */
#define EI_OSABI	7		/* OS ABI identification */
#define ELFOSABI_SYSV	0		/* Alias.  */
#define EI_ABIVERSION	8		/* ABI version */

/* Other header macors */
#define ET_NONE		0		/* No file type */
#define ET_REL		1		/* Relocatable file */
#define ET_EXEC		2		/* Executable file */
#define ET_DYN		3		/* Shared object file */
#define EV_CURRENT	1		/* Current version */
#define EM_SH		42		/* Hitachi SH */
#define EF_SH3		3		/* SH3 flags */




/* Program segment header.  */

typedef struct
{
	Elf32_Word	p_type;			/* Segment type */
	Elf32_Off	p_offset;		/* Segment file offset */
	Elf32_Addr	p_vaddr;		/* Segment virtual address */
	Elf32_Addr	p_paddr;		/* Segment physical address */
	Elf32_Word	p_filesz;		/* Segment size in file */
	Elf32_Word	p_memsz;		/* Segment size in memory */
	Elf32_Word	p_flags;		/* Segment flags */
	Elf32_Word	p_align;		/* Segment alignment */
} Elf32_Phdr;

/* Legal values for p_type (segment type).  */
#define	PT_NULL		0		/* Program header table entry unused */
#define PT_LOAD		1		/* Loadable program segment */
#define PT_DYNAMIC	2		/* Dynamic linking information */

/* Legal values for p_flags (segment flags).  */
#define PF_X		(1 << 0)	/* Segment is executable */
#define PF_W		(1 << 1)	/* Segment is writable */
#define PF_R		(1 << 2)	/* Segment is readable */



/* Section header.  */

typedef struct
{
	Elf32_Word	sh_name;		/* Section name (string tbl index) */
	Elf32_Word	sh_type;		/* Section type */
	Elf32_Word	sh_flags;		/* Section flags */
	Elf32_Addr	sh_addr;		/* Section virtual addr at execution */
	Elf32_Off	sh_offset;		/* Section file offset */
	Elf32_Word	sh_size;		/* Section size in bytes */
	Elf32_Word	sh_link;		/* Link to another section */
	Elf32_Word	sh_info;		/* Additional section information */
	Elf32_Word	sh_addralign;		/* Section alignment */
	Elf32_Word	sh_entsize;		/* Entry size if section holds table */
} Elf32_Shdr;

#endif /* __ELF_H__ */
