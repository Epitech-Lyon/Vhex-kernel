OUTPUT_FORMAT("elf32-sh", "elf32-sh", "elf32-sh")
OUTPUT_ARCH(sh3)

/*
** Linker script for user executables.
*/
MEMORY
{
	/* virtual memory, read-write segment */
	userram (WX) : o = 0x00000000, l = 256k
}

SECTIONS
{
	/* Code */
	.text : {
		*(.text);
		*(.text.*);

	} > userram

	/* Read-only sections */
	.rodata : {
		/* Read-Only data */
		*(.rodata);
		*(.rodata.*);

		/* Dynamic symbols */
		*(.hash)
		*(.dynsym)
		*(.dynstr)
		*(.dynbss)
		*(.dynamic)

		/*  Procedure Linkage Table */
		*(.plt)

		/* GLobal Offset Table */
		*(.got.plt)
		*(.got.*)
		*(.got)

		/* ???? */
		*(.rofixup)
	} > userram

	/* Relocatable sections */
	.rela.dyn : {
		*(.rela.plt)
		*(.rela.got)
		*(.rela.got.*)
		*(.rela.*)
		*(.rela.text)
		*(.real.data)
	} > userram

	/* readable / writable data */
	.data ALIGN(4) : {

		/* Data sections */
		*(.data);
		*(.data.*);

		/* bss section included to avoid missaligned segment */
		*(.bss);
		*(.bss.*);
		*(COMMON);
	} > userram

	/* unwanted section */
	/DISCARD/ : {
		*(.gnu.*)
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
		*(.interp)
	}
}
