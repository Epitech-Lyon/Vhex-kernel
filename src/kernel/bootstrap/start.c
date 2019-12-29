#include <stdint.h>
#include <stddef.h>
#include <kernel/context.h>
#include <kernel/atomic.h>
#include <kernel/types.h>
#include <kernel/process.h>
#include <lib/display.h>
#include <lib/string.h>

// Internal symbols
mpu_t current_mpu = MPU_UNKNOWN;

// Internal hardware context.
fx9860_context_t casio_context;
fx9860_context_t vhex_context;

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
extern uint32_t brom;
extern uint32_t srom;
extern uint32_t bctors;
extern uint32_t ectors;
extern uint32_t bdtors;
extern uint32_t edtors;

// Internal functions.
extern void vhex_context_set(void);
extern void kernel_switch(common_context_t *context);
extern mpu_t mpu_get(void);
extern int main(void);


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
		rom = (void*)((uint32_t)rom + 1024);
		size = size - 1024;
	}
}

/* section_execute() - Used to execute contructors and destructors */
static void section_execute(void *bsection, void *esection)
{
	while ((uint32_t)bsection < (uint32_t)esection)
	{
		((void (*)(void))*((uint32_t*)bsection))();
		bsection = (void*)((uint32_t)bsection + 4);
	}
}


/* start() - Kernel entry point */
__attribute__((section(".pretext")))
int start(void)
{
	int error;

	// Wipe .bss section and dump .data / Vhex sections
	memset(&bbss, 0x00, (size_t)&sbss);
	memcpy(&bdata_ram, &bdata_rom, (size_t)&sdata);

	// Check MPU hardware.
	current_mpu = mpu_get();
	if (current_mpu != MPU_SH7305)
	{
		return (0);
	}

	// Execute constructor.
	section_execute(&bctors, &ectors);

	// Load UBC / VBR space.
	memcpy(&bubc_ram, &bubc_rom, (size_t)&subc);
	memcpy(&bvhex_ram, &bvhex_rom, (size_t)&svhex);

	// Casio do not load all add-in
	// page. So we need to "force" update TLB
	// before switching the VBR.
	rom_explore(&brom, (int32_t)&srom);

	// Save Casio's hardware context and set
	// Vhex hardware context.
	// @note:
	// 	This operation should be atomic
	// because if an interruption or exception
	// occur during the hardware context change
	// the calculator will crash.
	// And this is why between each `atomic_start`
	// and `atomic_end()` the code *SHOULD* be
	// exception safe.
	atomic_start();
	fx9860_context_save(&casio_context);
	vhex_context_set();
	atomic_end();

	// Create first process: Vhex.
	uint32_t ssr = atomic_start();
	pid_t vhex_pid = process_create("Vhex");
	process_t *vhex_process = process_get(vhex_pid);
	vhex_process->context.spc = (uint32_t)&main;
	vhex_process->context.ssr = ssr;

	// Switch to first process.
	kernel_switch(&vhex_process->context);

	// normally the kernel SHOULD not
	// arrive here.
	while (1)
	{
		__asm__ volatile ("sleep");
	}
}
