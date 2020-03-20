#ifndef __LIB_STDIO_H__
# define __LIB_STDIO_H__

#include <stddef.h>
#include <stdint.h>
#include <stdarg.h>

/* vsprintf(), sprintf() - formatted output conversion. */
extern int vsprintf(char *str, char const *format, va_list ap);
extern int sprintf(char *str, char const *format, ...);

/* dprintf(), printf() - display formatted output */
extern int printf(const char *format, ...);
extern int dprintf(int fd, const char *format, ...);
extern int vdprintf(int fd, const char *format, va_list ap);

/* putx() - display char / string */
extern int putchar(int c);
extern int puts(const char *s);

#endif /*__LIB_STDIO_H__*/
