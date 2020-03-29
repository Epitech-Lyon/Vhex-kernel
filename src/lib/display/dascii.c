#include <display.h>

static void font_draw_core(uint32_t *vram, struct font_s *font, struct font_block_s *fblock)
{
	uint8_t vram_offset_y;
	uint32_t pixel_cursor;
	uint8_t pixel_test;
	int x;
	int y;

	// Calculate VRAM buffer starting position.
	// @note:
	// 	The screen width size is always 128 and we
	// use 4-aligned Video RAM so 32 pixels per "slot"
	// and 128 / 32 = 4.
	// y * 4 can be optimised by used shift operator,
	// this is why we use y << 2 because 2^2 = 4.
	vram_offset_y = fblock->y << 2;

	// Draw character, pixer per pixel... (x_x)
	// TODO: update me !!
	y = -1;
	while (++y < fblock->height)
	{
		x = -1;
		while (++x < fblock->width)
		{
			// Get bitmap pixel test.
			pixel_cursor = fblock->bitmap.x + fblock->bitmap.y + x;
			pixel_test = 0x80 >> (pixel_cursor & 0x07);

			// Check if the pixel is block or white.
			if (pixel_test & font->bitmap.raw[pixel_cursor >> 3])
			{
				vram[((fblock->x + x) >> 5) + vram_offset_y] |=
					0x80000000 >> ((fblock->x + x) & 31);
			}
		}

		// Update internal offset.
		fblock->bitmap.y = fblock->bitmap.y + font->bitmap.width; 
		vram_offset_y = vram_offset_y + 4;
	}
}


/* dascii() - Draw ASCII character into Video RAM */
void dascii(display_t *disp, int x, int y, char const c)
{
	struct font_block_s fblock;

	// Check obvious error.
	if (x > (int)disp->display.width || y > (int)disp->display.height)
		return;

	// Calculate the charactere position (bitmap)
	fblock.bitmap.y = (c / disp->nb_char_width) * (disp->font->bitmap.width * disp->font->bitmap.cheight);
	fblock.bitmap.x = (c - ((c / disp->nb_char_width) * disp->nb_char_width)) * disp->font->bitmap.cwidth;

	// Calculate real pixel position
	x = x * (disp->font->font.width + 1);
	y = y * (disp->font->font.height + 1);

	// Check X axis culling
	// and get font "block" width.
	if (x < 0)
	{
		fblock.bitmap.x = fblock.bitmap.x + x;
		fblock.width = disp->font->font.width + x;
		fblock.x = 0;
	} else {
		fblock.width = disp->font->font.width;
		if (x + (int)disp->font->font.width >= (int)disp->display.width)
			fblock.width = disp->display.width - x - 1;
		fblock.x = x;
	}

	// Check Y axis culling.
	// and get font "block" height.
	if (y < 0)
	{
		fblock.height = disp->font->font.height + y;
		fblock.bitmap.y = fblock.bitmap.y + ((-y) * disp->font->bitmap.width);
		fblock.y = 0;
	} else {
		fblock.height = disp->font->font.height;
		if (y + (int)disp->font->font.height >= (int)disp->display.height)
			fblock.height = disp->display.height - y - 1;
		fblock.y = y;
	}

	// Check useless draw.
	if (fblock.width < 0 || fblock.height < 0)
		return;

	// Draw ASCII character.
	font_draw_core(disp->vram, disp->font, &fblock);
}
