#ifndef __STRING_H__
# define __STRING_H__

#include <types.h>
#include <stdarg.h>

#define WEAK __attribute__((weak))

//#ifndef DEBUG
WEAK void *memset(void *s, int c, size_t n);
#ifndef DEBUG
WEAK void vsprintf(char *str, char const *format, va_list ap);
WEAK void sprintf(char *str, char const *format, ...);
#endif /* DEBUG */
WEAK char *strcat(char *dest, char const *src);
WEAK int strcmp(const char *s1, const char *s2);
WEAK char *strncpy(char *dest, char const *str, size_t size);
WEAK char *strcpy(char *dest, char const *src);
WEAK size_t strlen(char const *str);

// Custom functions
int strtotab(int *argc, char ***argv, char const *str);
void strtotab_quit(int *argc, char ***argv);

uint32_t atoi_base(char const *str, int base);
//#endif /* DEBUG */

#endif /*__STRING_H__*/
