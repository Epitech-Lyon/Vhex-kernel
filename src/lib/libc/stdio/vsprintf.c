#include <lib/stdio.h>
#include <stdarg.h>

static size_t strbase(char *dest, uint32_t nb, size_t size, int base)
{
	uint32_t tmp;
	uint8_t neg;

	neg = 0;
	if (base == 10 && (int32_t)nb < 0){
		*dest = '-';
		nb = -nb;
		neg = 1;
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
	tmp = size + neg;
	while ((int)(--size) >= neg){
		*(dest + size) = ((nb % base) + '0') + (39 * ((nb % base) > 9));
		nb /= base;
	}
	*(dest + tmp) = '\0';
	return (tmp);

}

static size_t strndump(char *dest, char const *src, size_t size)
{
	size_t i;

	if (src == NULL)
		return (0);
	i = -1;
	while (++i < size && src[i] != '\0')
		dest[i] = src[i];
	dest[i] = '\0';
	return (i);
}

//TODO: update me !!
int vsprintf(char *str, char const *format, va_list ap)
{
	const char *tmp;
	void  *sstr;
	size_t size;
	size_t len;
	int nb;

	if (format == NULL || str == NULL)
		return (-1);
	sstr = str;
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
		if (format[len] == '*'){
			size = va_arg(ap, size_t);
			format = format + 1;
		}
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
			tmp = va_arg(ap, const char *);
			/* TODO: find better way to do the job */
			str += strndump(str, tmp, (size == 0) ? 1024 : size);
			format += len + 1;
			continue;
		}
		if (format[len] == 'd'){
			nb = va_arg(ap, int);
			str += strbase(str, nb, size, 10);
			format += len + 1;
			continue;
		}
		if (format[len] == 'x'){
			nb = va_arg(ap, int);
			str += strbase(str, nb, size, 16);
			format += len + 1;
			continue;
		}
		if (format[len] == 'p' || (format[len] == '#' && format[len + 1] == 'x')){
			nb = va_arg(ap, int);
			*(str++) = '0';
			*(str++) = 'x';
			str += strbase(str, nb, 8, 16);
			format += (format[len] == 'p') ? len + 1 : len + 2;
			continue;
		}
		*(str++) = '%';
	}
	*str = '\0';
	return ((int)((void *)str - (void *)sstr));
}
