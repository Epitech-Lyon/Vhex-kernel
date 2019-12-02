#include <lib/string.h>

size_t strlen(char const *str)
{
	size_t i;

	if (str == NULL)
		return (0);
	i = -1;
	while (str[++i] != '\0');
	return (i);
}
