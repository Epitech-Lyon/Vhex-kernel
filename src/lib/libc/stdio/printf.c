#include <lib/stdio.h>
#include <lib/unistd.h>

int printf(const char *format, ...)
{
	va_list ap;
	int ret;

	va_start(ap, format);
	ret = vdprintf(STDOUT_FILENO, format, ap);
	va_end(ap);
	return (ret);
}
