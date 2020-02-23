#ifndef __KERNEL_UTIL_STRING_H__
# define __KERNEL_UTIL_STRING_H__

#include <stddef.h>
#include <stdint.h>

/* memset() - fill memory with a constant byte. */
extern void *memset(void *s, int c, size_t n);
extern void *memcpy(void *dest, const void *src, size_t n);

/* strcat() - concatenate two string */
extern char *strcat(char *dest, char const *src);

/* strcmp() - compare two strings */
extern int strcmp(const char *s1, const char *s2);
extern int strncmp(const char *s1, const char *s2, size_t n);

/* strcpy(), strncpy() - copy a string. */
extern char *strncpy(char *dest, char const *str, size_t size);
extern char *strcpy(char *dest, char const *src);

/* strlen - calculate the lenght of a string. */
extern size_t strnlen(char const *str, size_t maxlen);
extern size_t strlen(char const *str);

/* strchr - find the first / last occurent of the char c */
extern char *strchr(const char *s1, int c);
extern char *strchrnul(const char *s1, int c);
extern char *strrchr(const char *s1, int c);

#endif /*__KERNEL_UTIL_STRING_H__*/
