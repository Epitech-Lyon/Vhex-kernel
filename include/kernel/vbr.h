#ifndef __KERNEL_VBR_H__
# define __KERNEL_VBR_H__

#include <stddef.h>
#include <stdint.h>

// Vbr helper.
extern void *vbr_set(void *vbr_new);
extern void *vbr_get(void);

#endif /*__KERNEL_VBR_H__*/
