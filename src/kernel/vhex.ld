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
	ram	(WX)	: o = 0x88040000, l = 252k
	rom	(RX)	: o = 0x00300200, l = 512k
	ilram	(WX)	: o = 0xe5200000, l = 4k
	xram	(WX)	: o = 0xe5007000, l = 8k
}

PHDRS
{
	text	PT_LOAD ;
	data	PT_LOAD ;
	bss	PT_NULL ;
	ubc	PT_LOAD ;
	vbr	PT_LOAD ;
}

SECTIONS
{
	/*
	** ROM sections
	*/
	_brom = 0x00300000;
	_srom = 0x200 + SIZEOF(.text) +
			SIZEOF(.data) +
			SIZEOF(.vhex) +
			SIZEOF(.ubc);

	. = ORIGIN(rom);
	.text	: {
		/* Source Code */
		*(.pretext)
		*(.text)

		/* Read-Only data */
		. = ALIGN(4);
		*(.rodata)
		*(.rodata.*)
	
		/* Constructors, destructors */
		_bctors = ALIGN(4) ;
		*(SORT_BY_INIT_PRIORITY(.ctors.*))
		*(.ctors);
		_ectors = . ;
		_bdtors = ALIGN(4) ;
		*(SORT_BY_INIT_PRIORITY(.dtors.*))
		*(.dtors);
		_edtors = . ;

		/* File System */
		_bfilesystem_section = ALIGN(4) ;
		*(.filesystem);
		_efilesystem_section = . ;
	} > rom : text




	/*
	** RAM sctions
	*/
	. = ORIGIN(ram);

	/* BootStrap Stack: should be wiped later ! */
	/* (we force the section to be non loadable when the program is run) */
	.bss (NOLOAD) : {
		_bbss = . ;
		*(.bss)
		*(COMMON)
		. = ALIGN(4);
	} > ram : bss
	_sbss = SIZEOF(.bss);

	/* Read-write data going to RAM */
	.data ALIGN(4) : ALIGN(4) {
		_bdata_rom = LOADADDR(.data) ;
		_bdata_ram = . ;
	
		*(.data)

		_ram_start = ALIGN(1024);
	} > ram AT> rom : data
	_sdata = SIZEOF(.data);



	/*
	** VBR space !
	*/
	. = ORIGIN(xram);
	/* Interrupt / exception handlers */
	.vhex ALIGN(4) : SUBALIGN(4) {
		_bvhex_rom = LOADADDR(.vhex) ;
		_bvhex_ram = . ;
		_vhex_vbr = . - 0x100;
		*(.vhex.exception) ;
		. = _vhex_vbr + 0x400 ;
		*(.vhex.tlb) ;
		. = _vhex_vbr + 0x600 ;
		*(.vhex.interrupt) ;
		. = ALIGN(4);
	} > xram AT> rom : vbr
	_svhex = SIZEOF(.vhex);




	/*
	** DBR space !
	*/
	. = ORIGIN(ilram);
	.ubc ALIGN(4) : ALIGN(4) {
		_bubc_rom = LOADADDR(.ubc) ;
		_bubc_ram = . ;
		
		*(.ubc.handler)
		*(.ubc)

		. = ALIGN(4);
	} > ilram AT> rom : ubc
	_subc = SIZEOF(.ubc);
	



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
