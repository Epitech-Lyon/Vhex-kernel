#ifndef __KERNEL_UTIL_KMEM_H__
# define __KERNEL_UTIL_KMEM_H__

#include <stddef.h>
#include <stdint.h>

// Kernel memomries allocation function wrapper
extern void *kmem_alloc(size_t size);
extern void *kmem_realloc(void *ptr, size_t size);
extern void kmem_free(void *ptr);
extern void kmem_debug(void);

#endif /*__KERNEL_UTIL_KMEM_H__*/
