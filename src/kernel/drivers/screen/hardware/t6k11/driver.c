#include <kernel/drivers/screen.h>
#include <kernel/bits/driver.h>

// Internal drivers
extern void t6k11_variant_lcd_driver(void *vram);
extern void t6k11_lcd_driver(void *vram);

//TODO: save screen context !!
static void t6k11_driver_install(void)
{
	extern void (*screen_driver)(void *vram);

	// The T6K11 variant which appear during
	// the OS >= 03.00.0000, so check it
	// @note: The OS version is stored at 0xa0010021
	// TODO: get the OS version properly !!
	if (*(uint8_t*)0xa0010021 == '3') {
		screen_driver = (void*)&t6k11_variant_lcd_driver;
		return;
	}

	// Default driver
	screen_driver = &t6k11_lcd_driver;
}

static void t6k11_driver_uninstall(void)
{
	//FIXME: restore screen context !!!
}

// Create driver object
VHEX_DRIVER(4, driver_screen_t6k11) = {
	.arch = MPU_SH7305,
	.install = &t6k11_driver_install,
	.uninstall = &t6k11_driver_uninstall,
	.name = "t6k11"
};
