#ifndef __KERNEL_UTIL_EXTRA_H__
# define __KERNEL_UTIL_EXTRA_H__

#include <stddef.h>
#include <stdint.h>

// DeBug Register abstract
extern void *dbr_set(void *dbr_new);
extern void *dbr_get(void);

// SH4-instruction
extern void icbi(void *area);

// Vector Based Register abstract.
extern void *vbr_set(void *vbr_new);
extern void *vbr_get(void);

#endif /*__KERNEL_UTIL_EXTRA_H__*/
