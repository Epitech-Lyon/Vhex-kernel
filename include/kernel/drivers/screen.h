#ifndef __KERNEL_DRIVERS_SCREEN_H__
# define __KERNEL_DRIVERS_SCREEN_H__

#include <stddef.h>
#include <stdint.h>

// Internal enumeration for the
// screen getter
typedef enum {
	SCREEN_WIDTH,
	SCREEN_HEIGHT
} screen_getter_t;

// Internal hardware abstract
extern size_t screen_get(screen_getter_t getter);
extern void (*screen_update)(void *vram);

#endif /*__KERNEL_DRIVERS_SCREEN_H__*/
