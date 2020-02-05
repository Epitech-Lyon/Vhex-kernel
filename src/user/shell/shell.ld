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


	.relocgot : {
		BRELOC_GOT = . ;
		*(.got.plt)
		*(.got)
		ERELOC_GOT = . ;
	}

	/* unwanted section */
	/DISCARD/ : {
		*(.debug_info)
		*(.debug_abbrev)
		*(.debug_loc)
		*(.debug_aranges)
		*(.debug_ranges)
		*(.debug_line)
		*(.debug_str)
		*(.jcr)
		*(.eh_frame_hdr)
		*(.eh_frame)
		*(.comment)
	}
}
