#ifndef __KERNEL_UTIL_DEBUG_H__
# define __KERNEL_UTIL_DEBUG_H__

#include <stddef.h>
#include <stdint.h>
#include <kernel/util/draw.h>

// Wait debug (dirty)
#define DBG_WAIT for(int i = 0 ; i < 3000000 ; i++)

// Prototype
void printk(int x, int y, char const *str, ...);

#endif /*__KERNEL_UTIL_DEBUG_H__*/
