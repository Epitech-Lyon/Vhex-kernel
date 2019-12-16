#include <lib/display.h>
#include <kernel/font.h>


void dascii(int x, int y, char const c)
{
	font_draw(x, y, c);
}
