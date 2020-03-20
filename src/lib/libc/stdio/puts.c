#include <lib/stdio.h>
#include <lib/string.h>
#include <lib/unistd.h>

int puts(const char *s)
{
	size_t size;
	size_t n;

	size = strlen(s);
	n = write(STDOUT_FILENO, s, size);
	return (-(n == size));
}
