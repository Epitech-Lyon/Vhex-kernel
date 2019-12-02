#include <lib/stdio.h>

void sprintf(char *str, char const *format, ...)
{
	va_list ap;

	va_start(ap, format);
	vsprintf(str, format, ap);
	va_end(ap);
}
