#include <string.h>

static INLINE int check_base(char n, int base)
{
	char max;

	max = (base <= 10) ? '0' + base - 1 : ('a' + base - 11);
	return ((max < n));
}

uint32_t atoi_base(char const *str, int base)
{
	uint32_t result;
	size_t start;

	start = -1;
	result = 0;
	while (str[++start] != '\0' && check_base(str[start], base) == 0){
		result *= base;
		if (str[start] <= '9')
			result += str[start] - '0';
		else
			result += (str[start] - 'a') + 10;
	}
	return (result);
}
