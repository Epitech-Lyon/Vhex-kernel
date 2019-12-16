#include <lib/stdio.h>
#include <lib/display.h>
#include <kernel/devices/ubc.h>
#include <kernel/devices/tty.h>
#include <kernel/hardware/power.h>
#include <kernel/syscall.h>

// TODO: remove me !!
extern void test(void);

//TODO: add shell.
int main(void)
{
	char input[12];

	// Sleep test.

	// Open TTY.
	tty_open();
	

	// Entry ! :D
	tty_write("Boot Complete !\n", 16);

	// Keyboard test.
	while (1)
	{
		tty_write(">", 1);
		tty_read(input, 12);
	}

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

	return (0);
}
