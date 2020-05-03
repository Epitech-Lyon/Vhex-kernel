#include <kernel/drivers/vbr.h>
#include <kernel/bits/driver.h>
#include <string.h>

// Internal globals for abstractions
void (*int_handler[VBR_INTERRUPT_NB])(void);
int vbr_driver_is_installed = 0;
void *old_vbr;

// TODO: save area before dump ?
static void vbr_driver_install(void)
{
	extern uint32_t vhex_vbr;
	extern uint32_t bvhex_ram;
	extern uint32_t bvhex_rom;
	extern uint32_t svhex;

	// Save and switch the VBR
	// @note: The VBR swtich *SHOULD* be atomic because
	// one exepction / interrupt will crash the calculator.
	old_vbr = vbr_set(&vhex_vbr);

	// Check if the VBR driver is already installed
	if (vbr_driver_is_installed != 0)
		return;

	// Dump VBR handlers (interrupt and exception)
	memcpy(&bvhex_ram, &bvhex_rom, (size_t)&svhex);

	// Initialize pseudo-abtraction
	// TODO: Find better way to do the job
	for (int i = 0 ; i < VBR_INTERRUPT_NB ; ++i)
		int_handler[i] = NULL;

	// Indicate that the driver is installed
	vbr_driver_is_installed = 1;
}

static void vbr_driver_uninstall(void)
{
	// restore old VBR
	vbr_set(old_vbr);
}

// Create driver object
VHEX_DRIVER(3, driver_vbr) = {
	.arch = MPU_SH7305,
	.install = &vbr_driver_install,
	.uninstall = &vbr_driver_uninstall,
	.name = "vbr"
};
