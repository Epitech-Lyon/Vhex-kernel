#include <kernel/drivers/screen.h>

// Driver install indicator
void (*screen_driver)(void *vram) = NULL;

void screen_update(void *vram)
{
	if (screen_driver != NULL)
		screen_driver(vram);
}
