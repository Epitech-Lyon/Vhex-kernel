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

// TODO: update me :(
size_t strlen(char const *str)
{
	int i;

	if (str == NULL)
		return (0);
	i = -1;
	while (str[++i] != '\0');
	return (i);
}

//TODO: update me :(
int strcmp(const char *s1, const char *s2)
{
	if (s1 == NULL || s2 == NULL)
		return (0);
	while (*s1 != '\0' && *s2 != '\0' && *s1 == *s2)
	{
		s1 += 1;
		s2 += 1;
	}
	return (*s1 - *s2);
}

//TODO: update me :(
int strncmp(const char *s1, const char *s2, size_t n)
{
	if (s1 == NULL || s2 == NULL)
		return (0);
	size_t i = -1;
	while (++i < n && s1[i] != '\0' && s2[i] != '\0' && s1[i] == s2[i]);
	return (s1[i] - s2[i]);
}

//TODO asm ?
char *strrchr(const char *s1, int c)
{
	void *saved;

	saved = NULL;
	for (int i = 0 ; s1[i] != '\0' ; i++)
	{
		if (s1[i] == (char)c)
			saved = (void *)&s1[i];
	}
	return (saved);
}
