/*#include <lib/display.h>

__attribute__((section(".glados.tlb"), interrupt_handler))
void tlb_handler(void)
{
	uint32_t spc;
	uint32_t ssr;
	uint32_t sr;


	// Get some registers's data.
	__asm__ volatile (
		"stc	spc, %0;"
		"stc	ssr, %1;"
		"stc	sr, %2"
		: "=r"(spc), "=r"(ssr), "=r"(sr)
		:
		:
	);

	// Write exception informations.
	dclear();
	dprint(0, 0,
		"Ho crap ! Exception !\n"
		"tra:    %#x\n"
		"expevt: %#x\n"
		"spc:    %#x\n"
		"ssr:    %#x\n"
		"sr:     %#x",
		*((uint32_t *)0xff000020),
		*((uint32_t *)0xff000024),
		spc,
		ssr,
		sr
	);
	dupdate();
	while (1);
}*/
