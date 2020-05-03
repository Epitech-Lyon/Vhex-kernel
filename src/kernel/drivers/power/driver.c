#include <kernel/hardware/power.h>
#include <kernel/bits/driver.h>
#include <kernel/util/extra.h>
#include <string.h>

// Internal globals for abstractions
struct __sh7305_power_context vhex_power_context;
struct __sh7305_power_context casio_power_context;
int power_driver_is_installed = 0;

// TODO: save area before dump ?
static void power_driver_install(void)
{
	// Save current context (Casio)
	casio_power_context.stbcr = SH7305_POWER.STBCR.LONG_WORD;
	casio_power_context.mstpcr[0] = SH7305_POWER.MSTPCR0.LONG_WORD;
	casio_power_context.mstpcr[1] = SH7305_POWER.MSTPCR1.LONG_WORD;
	casio_power_context.mstpcr[2] = SH7305_POWER.MSTPCR2.LONG_WORD;
	casio_power_context.bar = SH7305_POWER.BAR;

	// Check if we need to install the default Vhex context
	if (power_driver_is_installed != 0)
	{
		SH7305_POWER.STBCR.LONG_WORD = vhex_power_context.stbcr;
		SH7305_POWER.MSTPCR0.LONG_WORD = vhex_power_context.mstpcr[0];
		SH7305_POWER.MSTPCR1.LONG_WORD = vhex_power_context.mstpcr[1];
		SH7305_POWER.MSTPCR2.LONG_WORD = vhex_power_context.mstpcr[2];
		SH7305_POWER.BAR = vhex_power_context.bar;
		icbi((void*)0xa0000000);
		return;
	}

	// Setup new power context
	// Selecte SLEEP mode (the CPU is stopped but all
	// peripheral continue to operates)
	SH7305_POWER.STBCR.RSTBY = 0;
	SH7305_POWER.STBCR.STBY  = 0;
	SH7305_POWER.STBCR.USTBY = 0;

	// Start Vhex peripheral
	// @note: We can't stop peripherals that we don't
	// use because we don't known if Casio use them >_<
	SH7305_POWER.MSTPCR0.TLB    = 0;	// start TLB
	SH7305_POWER.MSTPCR0.IC     = 0;	// start instruction cache
	SH7305_POWER.MSTPCR0.OC     = 0;	// start operand cache
	SH7305_POWER.MSTPCR0.RS     = 0;	// start RS memory
	SH7305_POWER.MSTPCR0.IL     = 0;	// start IL memroy
	SH7305_POWER.MSTPCR0.SC     = 0;	// start secondary cache
	SH7305_POWER.MSTPCR0.INTC   = 0;	// start interrupt controller
	SH7305_POWER.MSTPCR0.TMU0   = 0;	// start Timer Unit 0
	SH7305_POWER.MSTPCR1.KEYSC  = 0;	// start Key Scan Interface
	SH7305_POWER.MSTPCR1.RTC    = 0;	// start Real Time Clock
	SH7305_POWER.MSTPCR2.LCD    = 0;	// start LCD module

	// Force CPU update
	icbi((void*)0xa0000000);

	// Indicate that the driver is installed
	power_driver_is_installed = 1;
}

static void power_driver_uninstall(void)
{
	// Save current context (Vhex)
	vhex_power_context.stbcr = SH7305_POWER.STBCR.LONG_WORD;
	vhex_power_context.mstpcr[0] = SH7305_POWER.MSTPCR0.LONG_WORD;
	vhex_power_context.mstpcr[1] = SH7305_POWER.MSTPCR1.LONG_WORD;
	vhex_power_context.mstpcr[2] = SH7305_POWER.MSTPCR2.LONG_WORD;
	vhex_power_context.bar = SH7305_POWER.BAR;

	// Restore Casio context
	SH7305_POWER.STBCR.LONG_WORD = casio_power_context.stbcr;
	SH7305_POWER.MSTPCR0.LONG_WORD = casio_power_context.mstpcr[0];
	SH7305_POWER.MSTPCR1.LONG_WORD = casio_power_context.mstpcr[1];
	SH7305_POWER.MSTPCR2.LONG_WORD = casio_power_context.mstpcr[2];
	SH7305_POWER.BAR = casio_power_context.bar;

	// Force CPU update
	icbi((void*)0xa0000000);
}

// Create driver object
VHEX_DRIVER(1, driver_power) = {
	.arch = MPU_SH7305,
	.install = &power_driver_install,
	.uninstall = &power_driver_uninstall,
	.name = "power"
};
