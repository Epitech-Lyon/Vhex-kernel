#include <kernel/drivers/screen.h>

/* screen_get() - Internal screen getter */
size_t screen_get(screen_getter_t getter)
{
	switch (getter)
	{
		case SCREEN_WIDTH: return (128);
		case SCREEN_HEIGHT: return (64);
	}
	return (0);
}
