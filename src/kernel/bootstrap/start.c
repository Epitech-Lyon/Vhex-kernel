#include <stdint.h>
#include <stddef.h>
#include <kernel/context.h>
#include <kernel/atomic.h>
#include <kernel/types.h>
#include <lib/display.h>
#include <lib/string.h>

// Internal symbols
//uint32_t gcc_fix[256];
//uint32_t *VRAM = gcc_fix;
mpu_t current_mpu = MPU_UNKNOWN;
const char *casio_os_version = (void*)0xa0010020;
fx9860_context_t casio_context;
uint32_t casio_vbr;

// Symbols defined by the linker
extern uint32_t bbss;
extern uint32_t sbss;
extern uint32_t bdata_ram;
extern uint32_t bdata_rom;
extern uint32_t sdata;
extern uint32_t bvhex_ram;
extern uint32_t bvhex_rom;
extern uint32_t svhex;
extern uint32_t vhex_vbr;

// Internal functions.
extern void section_wipe(uint32_t *section, size_t size);
extern void section_load(uint32_t *dest, uint32_t *src, size_t size);
extern uint32_t vbr_set(uint32_t new_vbr);
extern uint32_t vbr_get(void);
extern mpu_t mpu_get(void);
extern int main(void);
extern void vhex_context_set(void);


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

// VBR test.
	__asm__ volatile ("trapa #0");

	// Call high level abstraction.
	error = main();

	// Mask interrupt, change context and switch VBR.
	atomic_start();
	fx9860_context_restore(&casio_context);
	vbr_set(casio_vbr);
	atomic_end();

	// Return the abstraction error.
	return (error);
}

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
