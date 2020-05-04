#include <kernel/util/casio.h>
#include <kernel/util/atomic.h>
#include <kernel/bits/context.h>
#include <kernel/devices/earlyterm.h>
#include <kernel/driver.h>

// Internal informations
int casio_timer_id;

static void casio_keycode_inject(void)
{
	uint16_t keycode;

	keycode = 0x308;
	casio_Bkey_PutKeymatrix(&keycode);
	casio_TimerUninstall(casio_timer_id);
}

//@note:
// * mode 0 - Involve menu manually
// * mode 1 - Use GetKey_wait
// FIXME: WAIT DMA transfert !!
void casio_return_menu(int mode)
{
	unsigned int keycode;
	int row;
	int col;

	// unistall / restore all drivers
	drivers_uninstall(0);

	// If we should involve main menu manually
	// we need to setup one timer which will inject
	// Casio's [MENU] keycode to force the GetKey_wait syscall
	// to return in main menu
	if (mode == 0) {
		casio_timer_id = casio_TimerInstall(0, &casio_keycode_inject, 25);
		casio_TimerStart(casio_timer_id);
	}

	// Use GetKeyWait to wait key event (or keycode inject)
	do {
		casio_GetKeyWait(&row, &col, KEYWAIT_HALTON_TIMERON, 25, 0, &keycode);
	} while (mode == 1);
	
	// Re-install Vhex drivers
	drivers_install(0);
}
