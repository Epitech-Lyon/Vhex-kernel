#ifndef __KERNEL_DBR_H__
# define __KERNEL_DBR_H__

#include <stddef.h>
#include <stdint.h>

extern void *dbr_set(void *dbr_new);
extern void *dbr_get(void);

#endif /*__KERNEL_DBR_H__*/
