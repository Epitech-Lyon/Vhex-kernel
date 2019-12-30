#include <kernel/util.h>

// Handler prototype.
extern void keysc_handler(void);
extern void timer_handler(void);

__attribute__((section(".vhex.interrupt"), interrupt_handler))
void interrupt_handler(void)
{
	uint32_t intevt = *(uint32_t*)0xff000028;


	// Check KEYSC interrupt.
	if (intevt == 0xbe0)
	{
		keysc_handler();
		return;
	}
	if (intevt == 0x400 ||
			intevt == 0x410 ||
			intevt == 0x420)
	{
		timer_handler();
		return;
	}

	// Display error.
	kvram_clear();
	kvram_print(0, 0,
		"Ho crap ! Interrupt error !\n"
		"Interrupt ID (%#x)\n"
		"Error: handler not foud !\n",
		intevt
	);
	kvram_display();
	while (1);
}
