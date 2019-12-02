#include <lib/stdio.h>

static size_t strbase(char *dest, uint32_t nb, size_t size, int base)
{
	uint32_t tmp;

	if (base == 10){
		if ((int32_t)nb < 0){
			*(dest++) = '-';
			nb = -nb;
		}
	}
	if (size == 0){
		tmp = nb;
		while (tmp != 0){
			tmp /= base;
			size += 1;
		}
		if (size == 0)
			size = 1;
	}
	tmp = size;
	while ((int)(--size) >= 0){
		*(dest + size) = ((nb % base) + '0') + (39 * ((nb % base) > 9));
		nb /= base;
		
	}
	*(dest + tmp) = '\0';
	return (tmp);

}

static size_t strndump(char *dest, char const *str, size_t size)
{
	size_t i;

	if (str == NULL || dest == NULL)
		return (0);
	i = -1;
	while (++i < size && str[i] != '\0')
		dest[i] = str[i];
	dest[i] = '\0';
	return (i);
}

void vsprintf(char *str, char const *format, va_list ap)
{
	size_t size;
	size_t len;

	if (format == NULL)
		return;
	while (*format != '\0'){
		if (*format != '%'){
			*(str++) = *(format++);
			continue;
		}
		/* get size informations */
		size = 0;
		len = -1;
		format += 1;
		while (format[++len] >= '0' && format[len] <= '9')
			size = size * 10 + format[len] - '0';
		if (format[len] == '%'){
			*(str++) = '%';
			format += 1;
			continue;
		}
		if (format[len] == 'c'){
			*(str++) = (char)va_arg(ap, int);
			format += len + 1;
			continue;
		}
		if (format[len] == 's'){
			char const *tmp = va_arg(ap, char const *);
			/* TODO: find better way to do the job */
			str += strndump(str, tmp, (size == 0) ? 1024 : size);
			format += len + 1;
			continue;
		}
		if (format[len] == 'd'){
			int nb = va_arg(ap, int);
			str += strbase(str, nb, size, 10);
			format += len + 1;
			continue;
		}
		if (format[len] == 'x'){
			int nb = va_arg(ap, int);
			str += strbase(str, nb, size, 16);
			format += len + 1;
			continue;
		}
		if (format[len] == '#' && format[len + 1] == 'x'){
			int nb = va_arg(ap, int);
			*(str++) = '0';
			*(str++) = 'x';
			str += strbase(str, nb, 8, 16);
			format += len + 2;
			continue;
		}
		*(str++) = '%';
	}
	*str = '\0';
}
