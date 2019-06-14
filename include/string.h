#ifndef __STRING_H__
# define __STRING_H__

#include <types.h>
#include <stdarg.h>

// Define weak symbols 
#define WEAK __attribute__((weak))

/* memset() - fill memory with a constant byte. */
WEAK void *memset(void *s, int c, size_t n);

#ifndef DEBUG
/* vsprintf(), sprintf() - formatted output conversion. */
void vsprintf(char *str, char const *format, va_list ap);
void sprintf(char *str, char const *format, ...);
#endif /* DEBUG */

/* strcat() - concatenate two string */
WEAK char *strcat(char *dest, char const *src);

/* strcmp() - compare two strings */
WEAK int strcmp(const char *s1, const char *s2);

/* strcpy(), strncpy() - copy a string. */
WEAK char *strncpy(char *dest, char const *str, size_t size);
WEAK char *strcpy(char *dest, char const *src);

/* strlen - calculate the lenght of a string. */
WEAK size_t strlen(char const *str);



//
// Custom string function.
//
/* strtotab() - generate table of word and indicate the number of word. */
int strtotab(int *argc, char ***argv, char const *str);

/* strtotab_quit() - free all allocated memory by strtotab function. */
void strtotab_quit(int *argc, char ***argv);

/* atoi_base() - atoi wrapped function, but add the base number. */
uint32_t atoi_base(char const *str, int base);

#endif /*__STRING_H__*/
