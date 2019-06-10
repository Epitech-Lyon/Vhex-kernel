#include <string.h>

__attribute__((weak)) int strcmp(const char *s1, const char *s2)
{
	if (s1 == NULL || s2 == NULL)
		return (0);
	while (*s1 != '\0' && *s2 != '\0' && *s1 == *s2){
		s1 += 1;
		s2 += 1;
	}
	return (*s1 - *s2);
}
