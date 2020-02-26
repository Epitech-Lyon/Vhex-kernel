#include <kernel/drivers/keyboard.h>

/*keyboard_wait_event() - Wait keys events (press or release) */
void keyboard_wait_event(void)
{
	extern volatile uint8_t keylist_isUpdate;

	// Wait key list update.
	// @note:
	// 	To ensure reentrace and avoid data corruption
	// we should wait kernel indication.
	// TODO: explain correctly x)
	while (keylist_isUpdate == 0)
	{
		__asm__ volatile ("sleep");
	}
	keylist_isUpdate = 0;
}
