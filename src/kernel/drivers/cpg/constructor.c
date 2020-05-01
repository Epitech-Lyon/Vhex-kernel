#include <kernel/drivers/cpg.h>
#include <kernel/hardware/cpg.h>
#include <kernel/devices/earlyterm.h>

// Internal struct
struct cpg cpg_info;

__attribute__((constructor))
static void cpg_init(void)
{
	// Calculate FLL frequency (Khz)
	// @note: RCLK = 32 768 Hz
	cpg_info.fll_freq = SH7305_CPG.FLLFRQ.FLF * 32768;	// Hz
	cpg_info.fll_freq /=  (1 << SH7305_CPG.FLLFRQ.SELXM);	// Check FLL output division

	// Calculate PLL frequency (Khz)
	cpg_info.pll_freq = cpg_info.fll_freq * (SH7305_CPG.FRQCRA.STC + 1);

	// Calculate CPU clock frequency !
	cpg_info.cpu_freq = cpg_info.pll_freq / (1 << (SH7305_CPG.FRQCRA.IFC + 1));

	// Calculate BUS clock frequency !
	cpg_info.bus_freq = cpg_info.pll_freq / (1 << (SH7305_CPG.FRQCRA.BFC + 1));

	// Calculate Peripheral clock frequency !
	cpg_info.per_freq = cpg_info.pll_freq / (1 << (SH7305_CPG.FRQCRA.PFC + 1));
	

	// Debug
	earlyterm_write("Calibrate frequencies...\n");
	earlyterm_write(
		"* FLL freq: %d.%d Mhz\n"
		"* PLL freq: %d.%d Mhz\n"
		"* CPU freq: %d.%d Mhz\n"
		"* BUS freq: %d.%d Mhz\n"
		"* Per freq: %d.%d Mhz\n",
		cpg_info.fll_freq / 1000000, (((cpg_info.fll_freq - ((cpg_info.fll_freq / 1000000)) * 1000000)) + 999) / 1000,
		cpg_info.pll_freq / 1000000, (((cpg_info.pll_freq - ((cpg_info.pll_freq / 1000000)) * 1000000)) + 999) / 1000,
		cpg_info.cpu_freq / 1000000, (((cpg_info.cpu_freq - ((cpg_info.cpu_freq / 1000000)) * 1000000)) + 999) / 1000,
		cpg_info.bus_freq / 1000000, (((cpg_info.bus_freq - ((cpg_info.bus_freq / 1000000)) * 1000000)) + 999) / 1000,
		cpg_info.per_freq / 1000000, (((cpg_info.per_freq - ((cpg_info.per_freq / 1000000)) * 1000000)) + 999) / 1000
	);
	DBG_WAIT;
}
