#ifndef __KERNEL_DRIVER_H__
# define __KERNEL_DRIVER_H__

#include <stddef.h>
#include <stdint.h>

extern void drivers_install(int verbose);
extern void drivers_uninstall(int verbose);

#endif /*__KERNEL_DRIVER_H__*/
