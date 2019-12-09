#include <stdint.h>
#include <stddef.h>
#include <kernel/devices/display.h>
#include <kernel/devices/ubc.h>
#include <kernel/syscall.h>
#include <kernel/types.h>
#include <kernel/extra.h>
#include <lib/display.h>
#include <lib/string.h>

// Internal symbols
mpu_t current_mpu = MPU_UNKNOWN;

// Symbols defined by the linker
extern uint32_t bbss;
extern uint32_t sbss;
extern uint32_t bdata_ram;
extern uint32_t bdata_rom;
extern uint32_t sdata;
extern uint32_t bvhex_ram;
extern uint32_t bvhex_rom;
extern uint32_t svhex;
extern uint32_t bubc_ram;
extern uint32_t bubc_rom;
extern uint32_t subc;


// Internal functions.
extern mpu_t mpu_get(void);
extern void ubc_handler_pre(void);
extern void test(void);
extern int main(void);


__attribute__((section(".pretext")))
int start(void)
{
	unsigned int key;
	void *casio_dbr;

	// Wipe .bss section and dump .data / Vhex sections
	memset(&bbss, 0x00, (size_t)&sbss);
	memcpy(&bubc_ram, &bubc_rom, (size_t)&subc);
	memcpy(&bdata_ram, &bdata_rom, (size_t)&sdata);
	memcpy(&bvhex_ram, &bvhex_rom, (size_t)&svhex);

	// Get Casio's VRAM
	display_open();

/*	volatile uint32_t *bite = (void*)0xe5200000;

	dclear();
	dprint(0, 0, "UBC  = %p", &ubc_handler_pre);
	dprint(0, 1, "TEST = %#x", *bite);
	dprint(0, 2, "TEST = %#x", *(uint32_t*)&bubc_rom);
	dprint(0, 3, "TEST = %#x", (uint32_t)&bubc_ram);
	dprint(0, 4, "TEST = %#x", (uint32_t)&bubc_rom);
	dprint(0, 5, "TEST = %#x", (size_t)&subc);
	dupdate();
	while (1);*/

	// Check MPU hardware.
	current_mpu = mpu_get();
	if (current_mpu != MPU_SH7305)
	{
		return (0);
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

	// Vhex's job is finished, display log.
	dclear();
	dprint(0, 0, "Vhex end !!\nNow wait [MENU] key press !");
	dupdate();

	// Wait [MENU] key is press.
	while (1)
	{
		casio_GetKey(&key);
	}

	// Unused.
	return (0);
}


/* Vhex with kernel version.
__attribute__((section(".pretext")))
int start(void)
{
	int error;

	// Wipe .bss section and dump .data / Vhex sections
	section_wipe(&bbss, (size_t)&sbss);
	section_load(&bdata_ram, &bdata_rom, (size_t)&sdata);
	section_load(&bvhex_ram, &bvhex_rom, (size_t)&svhex);

	// Check MPU hardware.
	current_mpu = mpu_get();
	if (current_mpu != MPU_SH7305)
	{
		return (0);
	}

	// Mask interrupt, change context and switch VBR.
	atomic_start();
	fx9860_context_save(&casio_context);
	casio_vbr = vbr_set((uint32_t)&vhex_vbr);
	vhex_context_set();
	atomic_end();

	// Call high level abstraction.
	error = main();

	// Mask interrupt, change context and switch VBR.
	atomic_start();
	fx9860_context_restore(&casio_context);
	vbr_set(casio_vbr);
	atomic_end();

	// Return the abstraction error.
	return (error);
}*/

//
// initialize() - kernel entry
// This is the "real" entry of the kernel (after stack and CPU configuration).
// This part *SHOULD* be exception safe because the CPU block all exception and interruption,
// so if an exception occur during the "bootstrap" part, the processor will "crash".
//
/*void initialize(void)
{
	interrupt_block();
	printk(KERN_NOTICE, "Kernel initialization...\n");

	// Wipe BSS section !
	printk(KERN_NOTICE, "Wipe bss / dump data...\n");
	memcpy(&bdata_ram, &bdata_rom, (size_t)&sdata);
	memset(&bbss, 0x00, (size_t)&sbss);


	// Check MPU.
	current_mpu = mpu_get();
	if (current_mpu != MPU_SH7305)
	{
		printk(KERN_NOTICE, "ARCH ERROR !\n");
		int i = -1;
		while (++i < 500000);
		return (0);
	}

	// Dump kernel and data.
	printk(KERN_NOTICE, "Dump VBR space...\n");
	memcpy(&bvhex_ram, &bvhex_rom, (size_t)&svhex);

	// Initialize Interruption (extern module, VBR, handlers)
	printk(KERN_NOTICE, "Initialize hardware context...\n");
	sh7305_context_initialize();

	// Save / set new VBR.
	printk(KERN_NOTICE, "Switch VBR / SR register...\n");
	__asm__ volatile (
			"stc    vbr, %0;"
			"stc    sr, %1;"
			"ldc    %2, vbr;"
			"ldc    %3, sr"
			: "=r"(casio_vbr), "=r"(casio_sr)
			: "r"(&vhex_vbr), "r"(0x60000000)
			:
			);

	printk(KERN_NOTICE, "Initialize heap space...\n");
	pm_heap_initialize((void*)0x8806000, (void*)0x8808000, 32);

	// Kernel initialized !
	printk(KERN_NOTICE, "Kernel initialized !\n");
	interrupt_unblock();

	// Call shell.
	shell_entry();

	// Kernel exit part.
	printk(KERN_NOTICE, "Kernel exit !");
	interrupt_block();

	// restore Casio's context.
	printk(KERN_NOTICE, "Restore hardware context...\n");
	sh7305_context_restore();

	// Restore VBR / CPU configurations.
	printk(KERN_NOTICE, "Restore VBR / SR...\n");
	__asm__ volatile ("ldc %0, vbr" : : "r"(&casio_vbr) : );
	__asm__ volatile ("ldc %0, sr"  : : "r"(casio_sr) : );

	// Clean exit.
	interrupt_unblock();
	return (0);
}*/
