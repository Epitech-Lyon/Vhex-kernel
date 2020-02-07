#ifndef __KERNEL_UTIL_H__
# define __KERNEL_UTIL_H__

#include <stddef.h>
#include <stdint.h>

// String function
extern void *memset(void *s, int c, size_t n);
extern void *memcpy(void *dest, const void *src, size_t count);
extern char *strncpy(char *dest, char const *str, size_t size);
extern size_t strnlen(char const *str, size_t maxlen);
extern int strcmp(const char *s1, const char *s2);
extern int strncmp(const char *s1, const char *s2, size_t n);
extern char *strrchr(const char *s1, int c);
extern size_t strlen(char const *str);

// Video RAM functions
extern void kvram_clear(void);
extern void kvram_display(void);
extern void kvram_scroll(int lines);
extern void kvram_ascii(int x, int y, char const c);
extern void kvram_reverse(int x, int y, int width, int height);
extern void kvram_clr_str_area(int x, int y, int width, int height);

// Kernel printf-wrapper
extern void printk(int x, int y, char const *str, ...);

// Hardware specific function (do not use !)
extern void t6k11_lcd_driver(void *vram);
extern void t6k11_variant_lcd_driver(uint8_t *vram);

//---
//	Timer functions.
//---
#define TIMER_NUMBER	3
#define TIMER_UNUSED	0xff

struct timer_cache_s
{
	void *callback;
	volatile void *arg;
	uint8_t status;
};
extern int timer_uninstall(int timer_ID);
extern int timer_install(void *callback, void *arg, uint32_t ticks, uint8_t mode);
extern int timer_start(int timer_ID);

// Debug wait
#define DBG_WAIT for(int i = 0 ; i < 3000000 ; i++)

#endif /*__KERNEL_UTIL_H__*/
