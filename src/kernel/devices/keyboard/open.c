#include <kernel/devices/keyboard.h>

// Internal constructor.
extern void keycache_init(void);

int keyboard_open(void)
{
	keycache_init();
	return (0);
}
