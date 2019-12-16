#ifndef __STRING_H__
# define __STRING_H__

#include <stdint.h>
#include <stddef.h>

/* memset() - fill memory with a constant byte. */
void *memset(void *s, int c, size_t n);
void *memcpy(void *dest, const void *src, size_t n);

/* strcat() - concatenate two string */
char *strcat(char *dest, char const *src);

/* strcmp() - compare two strings */
int strcmp(const char *s1, const char *s2);

/* strcpy(), strncpy() - copy a string. */
char *strncpy(char *dest, char const *str, size_t size);
char *strcpy(char *dest, char const *src);

/* strlen - calculate the lenght of a string. */
size_t strnlen(char const *str, size_t maxlen);
size_t strlen(char const *str);

#endif /*__STRING_H__*/
