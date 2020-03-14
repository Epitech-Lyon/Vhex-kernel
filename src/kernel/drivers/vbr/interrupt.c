#include <kernel/devices/earlyterm.h>

// Handler prototype.
extern void keysc_handler(void);
extern void timer_handler(void);

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
	earlyterm_clear();
	earlyterm_write(
		"Ho crap ! Interrupt error !\n"
		"Interrupt ID (%#x)\n"
		"Error: handler not foud !\n",
		intevt
	);
	while (1);
}
