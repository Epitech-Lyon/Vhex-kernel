#ifndef __LIB_DISPLAY_H__
# define __LIB_DISPLAY_H__

#include <stddef.h>
#include <stdint.h>

// Draw primtives
extern void dclear(void);
extern void dprint(int x, int y, char const *str, ...);
extern void dascii(int x, int y, char const c);
extern void dreverse(int x, int y, int width, int height);
extern void dscroll(int line);
extern void dupdate(void);

#endif /*__LIB_DISPLAY_H__*/
