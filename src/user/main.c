/*#include <lib/stdio.h>
#include <lib/display.h>

// TODO: remove me !!
extern void test(void);

int main(void)
{
	dclear();
	dprint(0, 0, "Boot complete !");
	dupdate();
	while (1);


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
	//((void(*)(void))0x08100000)();
	//((void(*)(void))&vhex_dbr)();
	test();

	// Power OFF UBC module.
	ubc_close();
}*/
