OUTPUT_ARCH(sh3)
OUTPUT_FORMAT(elf32-sh)
ENTRY(_main)

/*
** Linker script for user executables.
*/
MEMORY
{
	/* virtual memory, read-write segment */
	userram (WX) : o = 0x00000000, l = 256k
}

PHDRS
{
	text PT_LOAD ;
	data PT_LOAD ;
}

SECTIONS
{
	.text : {
		*(.text);
		*(.text.*);
	} : text

	.rodata : {
		*(.rodata);
		*(.rodata.*);
	}

	.data ALIGN(4) : {
		*(.plt);
		*(.data);
		*(.data.*);

		/* bss section included to avoid missaligned segment */
		*(.bss);
		*(.bss.*);
		*(COMMON);
	} : data
}
