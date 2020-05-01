#ifndef __KERNEL_DRIVERS_CPG_H__
# define __KERNEL_DRIVERS_CPG_H__

#include <stddef.h>
#include <stdint.h>

struct cpg
{
	uint32_t fll_freq;
	uint32_t pll_freq;
	uint32_t cpu_freq;
	uint32_t bus_freq;
	uint32_t per_freq;
};

enum cpg_module
{
	CPG_FLL,
	CPG_PLL,
	CPG_CPU,
	CPG_BUS,
	CPG_PERIPHERAL
};

// Getter
extern uint32_t cpg_get_frequency(enum cpg_module module);

#endif /*__KERNEL_DRIVERS_CPG_H__*/
