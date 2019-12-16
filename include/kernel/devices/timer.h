#ifndef __KERNEL_DEVICES_TIMER_H__
# define __KERNEL_DEVICES_TIMER_H__

#include <stddef.h>
#include <stdint.h>

// TODO: MOVE ME !!!!!!!!
#define TIMER_NUMBER	3
#define TIMER_UNUSED	0xff

struct timer_cache_s
{
	void *callback;
	volatile void *arg;
	uint8_t status;
};

#endif /*__KERNEL_DEVICES_TIMER_H__*/
