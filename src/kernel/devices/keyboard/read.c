#include <kernel/devices/keyboard.h>

ssize_t keyboard_read(void *buffer, size_t count)
{
	extern struct keycache_s *keylist;
	extern volatile uint8_t keylist_isUpdate;

	// Wait key list update.
	while (keylist_isUpdate == 0)
	{
		__asm__ volatile ("sleep");
	}
	return (0);
}
