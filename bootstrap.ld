OUTPUT_ARCH(sh3)
OUTPUT_FORMAT(elf32-sh)
ENTRY(_initialize)

MEMORY
{
	rom	: o = 0x00300200, l = 512k
	ram	: o = 0x08100000, l = 8k
}

SECTIONS
{
	. = ORIGIN(rom);
	.pretext : {
		*(.pretext.entry)
		*(.pretext)
	} > rom

	.text	: {
		*(.text)
	} > rom

	.rodata	: SUBALIGN(4) {
		*(.rodata)
		. = ALIGN(4);
		_bcmd_cache = . ;
		*(.cmd.cache)
		_ecmd_cache = . ;
	} > rom


	. = ORIGIN(ram);
	.bss (NOLOAD) : {
		_bbss = . ;
		*(.bss)
		*(COMMON)
	} > ram : NONE
	_sbss = SIZEOF(.bss);

	.data ALIGN(4) : ALIGN(4) {
		_bdata_rom = LOADADDR(.data) ;
		_bdata_ram = . ;
		*(.data)
	} > ram AT> rom
	_sdata = SIZEOF(.data) ;

	/* unused sections */
	/DISCARD/ : {
		*(.comment)
		*(.comment.*)
	}
}
