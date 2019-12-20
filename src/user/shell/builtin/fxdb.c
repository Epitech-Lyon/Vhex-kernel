#include "builtin.h"
#include <kernel/devices/ubc.h>
#include <lib/display.h>

// TODO: remove me !!
extern void test(void);

VHEX_BUILTIN(fxdb)
{
	//TODO: handle parameter.
	(void)argc;
	(void)argv;

	dclear();
	dprint(0, 0, "FXDB - entry !!");
	dupdate();
	for (int i = 0 ; i < 9000000 ; i = i + 1);
	return (0);

	// Open User Break Controller.
	// @note:
	// 	This function is hardcoded to follow syscall
	// 	execution based on userland vitural address
	// 	0x08100000, reserved for add-in RAM.
	// 	The tested programe is staticaly linked by the
	// 	linker script.
	// 	This module is only on SH7305 - SH4 based MPU.
	// 	THis function SHOULD not be called !
	ubc_open();

	// Jump into the tested function.
	// @note:
	// 	Thus USC should be start after the jump.
	//
	test();

	// Power OFF UBC module.
	ubc_close();
	return (0);
}
