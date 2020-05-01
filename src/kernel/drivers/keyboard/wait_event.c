#include <kernel/drivers/keyboard.h>
#include <kernel/util/atomic.h>

/*keyboard_wait_event() - Wait keys events (press or release) */
void keyboard_wait_event(keyscan_t *list)
{
	extern volatile uint8_t keylist_isUpdate;
	extern struct keycache_s *keylist;
	struct keycache_s *keynode;
	int i;

	// Wait key list update.
	// @note : each tim the user press or relase key(s)
	// the hardware driver will set the "keylist_isUpdate" to 1.
	// TODO: schedule !!
	while (keylist_isUpdate == 0)
	{
		__asm__ volatile ("sleep");
	}

	// Start atomic operations
	atomic_start();

	// Dump all keys
	i = 0;
	keynode = keylist;
	while (keynode != NULL)
	{
		list[i].counter = keynode->counter;
		list[i].keycode = keynode->keycode;
		keynode = keynode->next;
		i = i + 1;
	}
	list[i].keycode = KEY_UNUSED;

	// Indicate that the key buffer has been readed
	keylist_isUpdate = 0;

	// Stop atomic operations
	atomic_stop();
}
