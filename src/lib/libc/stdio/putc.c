#include <lib/stdio.h>
#include <lib/unistd.h>

int putc(int c)
{
	char n;

	n = (char)c;
	write(STDOUT_FILENO, &n, 1);
	return (n);
}
