OUTPUT_ARCH(sh3)
OUTPUT_FORMAT(elf32-sh)
ENTRY(_start)

MEMORY
{
	/*
	** bootram is the RAM physical location for global variable
	** during the bootloader step.
	** This location is realy important because no Casio's OS data
	** should be overwrite (specially TLB information stored by Casio).
	*/
	bootram	(rwx)	: o = 0x88040000, l = 252k
	osram	(rwx)	: o = 0x8807f000, l = 4k
	rom  	(rx)	: o = 0x00300200, l = 512k
}

SECTIONS
{
	/*
	** ROM sections
	*/
	. = ORIGIN(rom);
	.text	: {
		*(.pretext)
		*(.text)

		. = ALIGN(4);
		*(.rodata)
		*(.rodata.*)
	} > rom


	/*
	** RAM sctions
	*/
	. = ORIGIN(bootram);

	/* BootStrap Stack: should be wiped later ! */
	/* (we force the section to be non loadable when the program is run) */
	.bss (NOLOAD) : {
		_bbss = . ;
		*(.bss)
		*(COMMON)
		. = ALIGN(4);
	} > bootram :NONE
	_sbss = SIZEOF(.bss);

	/* Read-write data going to RAM */
	.data	: ALIGN(4) {
		_bdata_rom = LOADADDR(.data) ;
		_bdata_ram = . ;
		*(.data)

		/* Video RAM space */
		_VRAM = ALIGN(4);
		. = (. + 1024);
	} > bootram AT> rom
	_sdata = SIZEOF(.data);


	/*
	** VBR space !
	*/
	/* Interrupt / exception handlers */
	.vhex : SUBALIGN(4) {
		_bvhex_rom = LOADADDR(.vhex) ;
		_bvhex_ram = . ;
		_vhex_vbr = . - 0x100;
		*(.vhex.exception) ;
		. = _vhex_vbr + 0x400 ;
		*(.vhex.tlb) ;
		. = _vhex_vbr + 0x600 ;
		*(.vhex.interrupt) ;
		. = ALIGN(4);
	} > osram AT> rom
	_svhex = SIZEOF(.vhex);

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
