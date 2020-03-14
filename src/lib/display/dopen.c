#include <lib/display.h>

int dopen(display_t *disp, const char *fontname)
{
	extern struct font_s default5x3;

	// TODO: handle font name
	(void)fontname;
	disp->font = &default5x3;

	// Display size is hardcoded for now
	disp->display.width = 128;
	disp->display.height = 64;

	// Pre-calculate internal value used to draw
	disp->nb_char_width = disp->font->bitmap.width + disp->font->bitmap.cwidth - 1;
	disp->nb_char_width = disp->nb_char_width / disp->font->bitmap.cwidth;
	return (0);
}
