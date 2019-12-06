#ifndef __KERNEL_DEVICES_UBC_H__
# define __KERNEL_DEVICES_UBC_H__

#include <stddef.h>
#include <stdint.h>

typedef struct ubc_context_s
{
	uint32_t reg[16];
	uint32_t gbr;
	uint32_t macl;
	uint32_t mach;
	uint32_t ssr;
	uint32_t spc;
} ubc_context_t;

// Primitives.
extern int ubc_open(void);
extern int ubc_close(void);

#endif /*__KERNEL_DEVICES_UBC_H__*/
