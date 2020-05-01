#include <kernel/drivers/cpg.h>

uint32_t cpg_get_frequency(enum cpg_module module)
{
	extern struct cpg cpg_info;

	switch (module)
	{
	case CPG_FLL: return (cpg_info.fll_freq);
	case CPG_PLL: return (cpg_info.pll_freq);
	case CPG_CPU: return (cpg_info.cpu_freq);
	case CPG_BUS: return (cpg_info.bus_freq);
	case CPG_PERIPHERAL: return (cpg_info.per_freq);
	}
	return (0);
}
