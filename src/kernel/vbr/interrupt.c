#include <lib/display.h>

__attribute__((section(".vhex.interrupt"), interrupt_handler))
void interrupt_handler(void)
{
	dclear();
	dprint(0, 0, "Interrupt handler (%#x)\n", *(uint32_t*)0xff000028);
	dupdate();
	while (1);
}
