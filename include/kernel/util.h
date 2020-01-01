#ifndef __KERNEL_UTIL_H__
# define __KERNEL_UTIL_H__

#include <stddef.h>
#include <stdint.h>

// String function
extern void *memset(void *s, int c, size_t n);
extern void *memcpy(void *dest, const void *src, size_t count);
extern char *strncpy(char *dest, char const *str, size_t size);
extern size_t strnlen(char const *str, size_t maxlen);

// Video RAM functions
extern void kvram_clear(void);
extern void kvram_display(void);
extern void kvram_scroll(int lines);
extern void kvram_reverse(int x, int y, int width, int height);
extern void kvram_print(int x, int y, char const *str);
extern void kvram_ascii(int x, int y, char const c);

// Kernel printf-wrapper
extern void printk(int x, int y, char const *str, ...);

// Hardware specific function (do not use !)
extern void t6k11_display(void *vram);

// Timer functions.
extern int timer_uninstall(int timer_ID);
extern int timer_install(void *callback, void *arg, uint32_t delay_ms, uint8_t mode);

// Debug wait
#define DBG_WAIT for(int i = 0 ; i < 3000000 ; i++)

#endif /*__KERNEL_UTIL_H__*/
