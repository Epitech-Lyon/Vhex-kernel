#ifndef __KERNEL_BITS_CONTEXT_H__
# define __KERNEL_BITS_CONTEXT_H__

#include <stddef.h>
#include <stdint.h>

// Define the SuperH CPU hardware context
struct cpu_context
{
	uint32_t reg[16];
	uint32_t gbr;
	uint32_t macl;
	uint32_t mach;
	uint32_t ssr;
	uint32_t spc;
	uint32_t pr;
};

#endif /*__KERNEL_BITS_CONTEXT_H__*/
