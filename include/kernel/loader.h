#ifndef __KERNEL_LOADER_H__
# define __KERNEL_LOADER_H__

#include <stddef.h>
#include <stdint.h>

// Function
extern void *loader(const char *path);

#endif /*__KERNEL_LOADER_H__*/
