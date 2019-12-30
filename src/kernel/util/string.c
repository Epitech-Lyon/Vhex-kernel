#include <kernel/util.h>

//TODO: update me :(
void *memset(void *s, int c, size_t n)
{
	while ((int)--n >= 0)
		((uint8_t*)s)[n] = c;
	return (s);
}

//TODO: update me :(
void *memcpy(void *dest, const void *src, size_t count)
{
	for (size_t i = 0 ; i < count ; i = i + 1)
		((uint8_t*)dest)[i] = ((uint8_t*)src)[i];
	return (dest);
}

// TODO: update me :(
char *strncpy(char *dest, char const *str, size_t size)
{
	size_t i;

	if (str == NULL || dest == NULL)
		return (0);
	i = -1;
	while (++i < size && str[i] != '\0')
		dest[i] = str[i];
	dest[i] = '\0';
	return (dest);
}

// TODO: update me :(
size_t strnlen(char const *str, size_t maxlen)
{
	size_t i;

	if (str == NULL)
		return (0);
	i = -1;
	while (str[++i] != '\0' && (size_t)i < maxlen);
	return (i);
}
