#include <lib/display.h>
#include <stdarg.h>

// Structure used to store
// internal data
struct dintern_s
{
	display_t *disp;
	size_t counter;
	va_list ap;
	int x;
	int y;
	int force; 
};

// Internal functions
static void disp_char(struct dintern_s *intern, char n);
static void dchar(struct dintern_s *intern, char spec);
static void dstr(struct dintern_s *intern, char spec);
static void duint(struct dintern_s *intern, char spec);
static void dint(struct dintern_s *intern, char spec);
static void dptr(struct dintern_s *intern, char spec);


// Internal actions list
void (*actions_list[26])(struct dintern_s *intern, char spec) = {
	NULL,		NULL,		dchar,		dint,
	NULL,		NULL,		NULL,		NULL,
	dint,		NULL,		NULL,		NULL,
	NULL,		NULL,		duint,		dptr,
	NULL,		NULL,		dstr,		NULL,
	duint,		NULL,		NULL,		duint,
	NULL,		NULL,
};


/* dchar() - display only one char */
static void dchar(struct dintern_s *intern, char spec)
{
	(void)spec;
	disp_char(intern, (char)va_arg(intern->ap, int));
}

/* dstr() - display string */
static void dstr(struct dintern_s *intern, char spec)
{
	char *str;

	(void)spec;
	str = va_arg(intern->ap, char *);
	while (*str != '\0')
	{
		disp_char(intern, *str);
		str = str + 1;
	}
}

/* dint() - Display unsigned interger */
static void duint(struct dintern_s *intern, char spec)
{
	const char hexa[] = "0123456789abcdef";
	char buffer[32];
	uint32_t num;
	int digits;
	int base;

	// Get appropriate base
	base = (spec == 'x') ? 16 : 10;

	// TODO: handle size
	num = va_arg(intern->ap, uint32_t);

	// Generate number (reverse)
	digits = 0;
	while (num != 0 || digits == 0) {
		buffer[digits++] = hexa[num % base];
		num = num / base;
	}
	while (--digits >= 0) {
		disp_char(intern, buffer[digits]);
	}
}

/* dptr() - Display pointer address */
static void dptr(struct dintern_s *intern, char spec)
{
	const char hexa[] = "0123456789abcdef";
	char buffer[32];
	uint32_t num;
	int digits;

	// Avoid unused parameter error
	(void)spec;

	// Display default string
	disp_char(intern, '@');
	disp_char(intern, '0');
	disp_char(intern, 'x');

	// TODO: handle size
	num = va_arg(intern->ap, uint32_t);

	// Generate number (reverse)
	digits = 0;
	while (num != 0 || digits < 8) {
		buffer[digits++] = hexa[num & 15];
		num = num >> 4;
	}
	while (--digits >= 0) {
		disp_char(intern, buffer[digits]);
	}
}

/* dint() - Display signed number (base 10) */
static void dint(struct dintern_s *intern, char spec)
{
	char buffer[32];
	int32_t num;
	int digits;

	// Avoid unused parameters
	(void)spec;

	// TODO: handle size
	num = va_arg(intern->ap, int32_t);
	
	// Check negative value
	// FIXME: maximum negative value !!!!!!
	if (num < 0) {
		disp_char(intern, '-');
		num = -num;
	}

	// Generate number (reverse)
	digits = 0;
	while (num != 0 || digits == 0) {
		buffer[digits++] = (num % 10) + '0';
		num = num / 10;
	}
	while (--digits >= 0) {
		disp_char(intern, buffer[digits]);
	}
}

/* line_discipline() - small line discipline */
static int line_discipline(struct dintern_s *intern, char n)
{
	// New line
	if (n == '\n')
	{
		intern->y = intern->y + intern->disp->font->font.height + 1;
		intern->x = 0;
		return (0);
	}
	// Horizontal tab
	if (n == '\t')
	{
		intern->x = intern->x / (intern->disp->font->font.width + 1);
		intern->x = (intern->x + (4 - (intern->x & 3)));
		intern->x = intern->x * (intern->disp->font->font.width + 1);
		return (0);
	}
	return (1);

}

/* disp_char() - display on character and update internal data */
static void disp_char(struct dintern_s *intern, char n)
{
	dascii(intern->disp, intern->x, intern->y, n);
	intern->x = intern->x + (intern->disp->font->font.width + 1);
	intern->counter = intern->counter + 1;
	intern->force = intern->force - 1;
}

/* dprint() - printf wrapper */
size_t dprint(display_t *disp, int x, int y, const char *format, ...)
{
	void (*action)(struct dintern_s *intern, char spec);
	struct dintern_s intern;
	void *sformat;

	// Get variable args entry
	va_start(intern.ap, format);

	// Generate pixel positions
	intern.x = x * (disp->font->font.width + 1);
	intern.y = y * (disp->font->font.height + 1);

	// Initialise internal data
	intern.counter = 0;
	intern.force = 0;

	// Display string char by char...
	format = format - 1;
	while (*(++format) != '\0')
	{
		// Check line discipline character
		if (line_discipline(&intern, *format) != 0)
			continue;

		// Check general character or forced character
		if (*format != '%' || intern.force > 0) {
			disp_char(&intern, *format);
			continue;
		}

		// Check "%%" format
		if (format[1] == '%') {
			disp_char(&intern, '%');
			format = format + 1;
			continue;
		}

		// Get format options
		// TODO: get options
		sformat = (void *)format;
		if (!(*format >= 'A' && *format <= 'Z') &&
				!(*format >= 'a' && *format <= 'z')) {
			intern.force = (uint32_t)format - (uint32_t)sformat;
			format = sformat;
			continue;
		}

		// Get action if possible
		action = actions_list[(*format | 0x20) - 'a'];
		if (action == NULL) {
			intern.force = (uint32_t)format - (uint32_t)sformat;
			format = sformat;
			continue;
		}

		// Call actions
		(*action)(&intern, *format);
	}
	return (intern.counter);
}
