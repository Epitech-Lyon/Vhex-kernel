#include <kernel/util/draw.h>

/* kvram_display() - Disaplay Video RAM into screen */
void kvram_display(void)
{
	extern void t6k11_variant_lcd_driver(void *vram);
	extern void t6k11_lcd_driver(void *vram);
	extern uint32_t vram[256];

	//TODO: handle screen hadware !
	//TODO: load dynamically screen driver during the boot
	if (*(uint8_t*)0xa0010021 == '3')
		t6k11_variant_lcd_driver((void*)vram);
	else
		t6k11_lcd_driver(vram);
}
