#include <kernel/drivers/screen.h>
#include <kernel/drivers/vbr.h>
#include <kernel/hardware/intc.h>
#include <kernel/hardware/tmu.h>
#include <kernel/bits/driver.h>

// Internal handlers
extern void sh7305_tmu_handler(void);

// Internal hardware context
struct __sh7305_tmu_context casio_tmu_context;
struct __sh7305_tmu_context vhex_tmu_context;
int tmu_driver_is_installed = 0;

static void tmu_driver_install(void)
{
	// Save current TMU timer's status and stop all
	casio_tmu_context.tstr = SH7305_TMU.TSTR.BYTE;
	SH7305_TMU.TSTR.BYTE = 0x00;			// Stop all timer

	// Configure TMU.
	for (int i = 0 ; i < 3 ; ++i)
	{
		// Save timmer context
		casio_tmu_context.timer[i].tcor = SH7305_TMU.TIMER[i].TCOR;
		casio_tmu_context.timer[i].tcnt = SH7305_TMU.TIMER[i].TCNT;
		casio_tmu_context.timer[i].tcr  = SH7305_TMU.TIMER[i].TCR.WORD;

		// Re-install driver if possible
		if (tmu_driver_is_installed != 0)
		{
			SH7305_TMU.TIMER[i].TCOR = vhex_tmu_context.timer[i].tcor;
			SH7305_TMU.TIMER[i].TCNT = vhex_tmu_context.timer[i].tcnt;
			SH7305_TMU.TIMER[i].TCR.WORD = vhex_tmu_context.timer[i].tcr;
		} else {
			SH7305_TMU.TIMER[i].TCOR = 0xffffffff;		// Reset Timer
			SH7305_TMU.TIMER[i].TCOR = 0xffffffff;		// Reset Timer
			SH7305_TMU.TIMER[i].TCR.UNF  = 0;		// Clear interrupt flag.
			SH7305_TMU.TIMER[i].TCR.UNIE = 0;		// Disable interrupt.
			SH7305_TMU.TIMER[i].TCR.TPSC = 0b0000;		// Select clock deviser.
		}
	}
	
	// Install interrupt handlers for each timer
	vbr_interrupt_set(INT_CODE_TMU0_TUNI0, &sh7305_tmu_handler);
	vbr_interrupt_set(INT_CODE_TMU0_TUNI1, &sh7305_tmu_handler);
	vbr_interrupt_set(INT_CODE_TMU0_TUNI2, &sh7305_tmu_handler);

	// Set interruptions priority
	SH7305_INTC.IPRA.TMU0_0 = 12;
	SH7305_INTC.IPRA.TMU0_1 = 12;
	SH7305_INTC.IPRA.TMU0_2 = 12;

	// Enable interruptions
	SH7305_INTC.IMCR4.TUNI2 = 1;
	SH7305_INTC.IMCR4.TUNI1 = 1;
	SH7305_INTC.IMCR4.TUNI0 = 1;

	// Restart timer if possible
	if (tmu_driver_is_installed != 0)
		SH7305_TMU.TSTR.BYTE = vhex_tmu_context.tstr;

	// Indicate that the driver has been installed
	tmu_driver_is_installed = 1;
}

// Restore context
static void tmu_driver_uninstall(void)
{
	// Save Vhex TMU timer's status and stop all
	vhex_tmu_context.tstr = SH7305_TMU.TSTR.BYTE;
	SH7305_TMU.TSTR.BYTE = 0x00;

	for (int i = 0; i < 3 ; ++i)
	{
		// Save timer context (Vhex)
		vhex_tmu_context.timer[i].tcor = SH7305_TMU.TIMER[i].TCOR;
		vhex_tmu_context.timer[i].tcnt = SH7305_TMU.TIMER[i].TCNT;
		vhex_tmu_context.timer[i].tcr  = SH7305_TMU.TIMER[i].TCR.WORD;

		// Restore Casio's TMU context
		SH7305_TMU.TIMER[i].TCOR = casio_tmu_context.timer[i].tcor;
		SH7305_TMU.TIMER[i].TCNT = casio_tmu_context.timer[i].tcnt;
		SH7305_TMU.TIMER[i].TCR.WORD = casio_tmu_context.timer[i].tcr;
	}

	// Restart timer
	SH7305_TMU.TSTR.BYTE = casio_tmu_context.tstr;
}

// Create driver object
VHEX_DRIVER(3, driver_tmu) = {
	.arch = MPU_SH7305,
	.install = &tmu_driver_install,
	.uninstall = &tmu_driver_uninstall,
	.name = "tmu"
};
