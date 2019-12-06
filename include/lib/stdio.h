#ifndef __LIB_STDIO_H__
# define __LIB_STDIO_H__

#include <stddef.h>
#include <stdint.h>
#include <stdarg.h>

/* vsprintf(), sprintf() - formatted output conversion. */
int vsprintf(char *str, char const *format, va_list ap);
void sprintf(char *str, char const *format, ...);

#endif /*__LIB_STDIO_H__*/
