#ifndef __KERNEL_BITS_DRIVER_H__
# define __KERNEL_BITS_DRIVER_H__

#include <kernel/bits/mpu.h>

// Macros used to declare new driver object
// @note:
//  Level 1: Should be installed first (power management)
//  Level 2: Higher priority (clock and bus control) 
//  Level 3: High priority (timer, rtc, ...)
//  Level 4: General drivers
#define VHEX_DRIVER(level, name)	\
	__attribute__((section(".driver." #level))) struct driver name

// Define driver object
struct driver
{
	mpu_t arch;
	void (*install)(void);
	void (*uninstall)(void);
	const char *name;
};

#endif /*__KERNEL_BITS_DRIVER_H__*/
