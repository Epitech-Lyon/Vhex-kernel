#ifndef __LIB_TIMER_H__
# define __LIB_TIMER_H__

#include <stddef.h>
#include <stdint.h>

// Internal functions.
extern int timer_install(void *callback, void *arg, uint32_t delay_ms, uint8_t mode);
extern int timer_uninstall(int timer_ID);

#endif /*__LIB_TIMER_H__*/
