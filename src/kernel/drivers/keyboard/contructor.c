#include <kernel/drivers/keyboard.h>

/*
** Internal cache, used like chained list.
**
** @note:
** The KEYSC have 6 key data 16-bits registers
** this is why we used 6 * 16 = 96 cache slot.
*/
struct keycache_s keycache[96];
struct keycache_s *keylist;

/*
** keycache_init()
**
** @info
** Constructor used by the kernel during the
** bootstrap part. It will initialize keycache
** and the chained list.
** This function sould not be called anymore.
*/
__attribute__((constructor))
static void keycache_init(void)
{
	int i;

	i = 96;
	while (--i >= 0)
	{
		keycache[i].keycode = KEY_UNUSED;
		keycache[i].next = NULL;
	}
	keylist = NULL;
}
