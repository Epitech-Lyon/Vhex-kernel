#include <utils.h>

//TODO: update me :(
__attribute__((weak)) void *memset(void *s, int c, size_t n)
{
	while ((int)--n >= 0)
		((uint8_t*)s)[n] = c;
	return (s);
}
