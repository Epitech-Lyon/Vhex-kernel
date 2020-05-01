#include <kernel/drivers/screen.h>

void screen_update(void *vram)
{
	extern void (*screen_driver)(void *vram);

	// Check driver validity
	if (screen_driver == NULL)
		return;
	screen_driver(vram);
}
