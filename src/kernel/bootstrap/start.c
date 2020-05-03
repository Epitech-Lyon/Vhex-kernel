// Types
#include <stdint.h>
#include <stddef.h>
#include <sys/types.h>
#include <kernel/bits/mpu.h>
// Internal helpers
#include <kernel/util/atomic.h>
#include <kernel/util/casio.h>
// Modules
#include <kernel/bits/context.h>
#include <kernel/process.h>
#include <kernel/syscall.h>
#include <kernel/scheduler.h>
#include <kernel/loader.h>
// Devices
#include <kernel/devices/earlyterm.h>
// Libs
#include <lib/display.h>
#include <lib/string.h>

// Symbols defined by the linker
extern uint32_t bbss;
extern uint32_t sbss;
extern uint32_t bdata_ram;
extern uint32_t bdata_rom;
extern uint32_t sdata;
extern uint32_t brom;
extern uint32_t srom;
extern uint32_t bctors;
extern uint32_t ectors;
extern uint32_t bdtors;
extern uint32_t edtors;

// Internal bootstrap funtions
extern mpu_t mpu_get(void);
extern void bootstrap_drivers_install(void);
extern void bootstrap_filesystem_init(void);

//
// rom_explore() - explore all add-in ROM part.
// @note:
//	For now, we can not use the MMU so we can
// not handle TLB miss. Casio load only the first 4ko
// of the add-in. To avoid TLB miss we explore all
// add-in binary to force Casio to generate all pages
// into the TLB.
//
static void rom_explore(volatile void *rom, int32_t size)
{
	uint8_t unused;

	(void)unused;
	while (size >= 0)
	{
		unused = *(volatile uint8_t*)rom;
		size = size - 1024;
		rom = rom + 1024;
	}
}

/* section_execute() - Used to execute contructors and destructors */
static void section_execute(void *bsection, void *esection)
{
	while (bsection < esection)
	{
		((void (*)(void))*((void**)bsection))();
		bsection = bsection + sizeof(void (*)(void));
	}
}

/* start() - Kernel entry point */
__attribute__((section(".pretext")))
int start(void)
{
	// Casio do not load all add-in page and we can not use the MMU.
	// So we need to "force" update TLB before switching the VBR to
	// avoid TLB miss exception that we can not handle for now.
	rom_explore(&brom, (int32_t)&srom);

	// Wipe .bss section and dump .data / Vhex sections
	memset(&bbss, 0x00, (size_t)&sbss);
	memcpy(&bdata_ram, &bdata_rom, (size_t)&sdata);

	// Start early terminal device
	// This device is used by the kernel to display
	// some logs on screen
	if (earlyterm_init() != 0)
		return (-1);
	earlyterm_clear();
	earlyterm_write("Kernel initialisation...\n");

	// Load all drivers on-the-fly
	bootstrap_drivers_install();

	// Execute constructor.
	// FIXME: remove me ?
	section_execute(&bctors, &ectors);


	// Initialize all File Systems and Virtual File System
	// TODO: rename into initramfs ?
	bootstrap_filesystem_init();


	//---
	//	Start first process !
	//---
	//TODO: move me !!
	
	// Load programe.
	earlyterm_write("Create first process...\n");
	struct process *vhex_process = process_create();
	if (vhex_process == NULL || loader(vhex_process, "/mnt/casio/VHEX/shell.elf") != 0)
	{
		// Display message.
		earlyterm_clear();
		earlyterm_write("Vhex fatal error !\n");
		earlyterm_write("Unable to create the first process !\n");
		if (vhex_process->context.spc == 0xffffffff)
			earlyterm_write("process_create() error\n");
		else
			earlyterm_write("File \"VHEX/shell.elf\" not found !\n");
		earlyterm_write("Press [MENU key]...\n");

		// Restore Casio's context and wait MENU key
		casio_return_menu(1);
	}


	//---
	//	Initialize sheduler !!
	//---
	//TODO: move me ?
	earlyterm_write("Initialize scheduler...\n");
	sched_initialize();
	sched_task_add(vhex_process);
	sched_start();

	// normally the kernel SHOULD / CAN not arrive here.
	earlyterm_write("Kernel job fini !\n");
	while (1) { __asm__ volatile ("sleep"); }
}
