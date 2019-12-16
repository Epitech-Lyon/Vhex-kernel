#include <kernel/devices/display.h>
#include <lib/display.h>
#include <stdarg.h>

void display_ioctl(uint32_t cmd, ...)
{
	extern int dcurx;
	extern int dcury;
	va_list ap;

	va_start(ap, cmd);
	switch (cmd)
	{
		case DISPLAY_IOCTL_GETX:
		{
			int *ret = va_arg(ap, int *);
			*ret = dcurx;
			break;
		}
		case DISPLAY_IOCTL_GETY:
		{
			int *ret = va_arg(ap, int *);
			*ret = dcury;
			break;
		}
		case DISPLAY_IOCTL_SETX:
		{
			dcurx = va_arg(ap, int);
			break;
		}
		case DISPLAY_IOCTL_SETY:
		{
			dcury = va_arg(ap, int);
			break;
		}
		case DISPLAY_IOCTL_CLEAR:
		{
			dclear();	// <- remove me ?
			break;
		}
		case DISPLAY_IOCTL_DISPLAY:
		{
			dupdate();	// <- remove me ?
			break;
		}
	}
	va_end(ap);
}
