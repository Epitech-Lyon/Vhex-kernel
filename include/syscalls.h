#ifndef __CASIO_H__
# define __CASIO_H__

#include <types.h>

struct rect
{
	int left;
	int top;
	int right;
	int bottom;
};


// Casio prototype.
void casio_GetKey(unsigned int *key);
void casio_Bdisp_AllClr_VRAM(void);
void casio_Bdisp_AreaClr_VRAM(const struct rect *buf);
void casio_PrintMini(size_t x, size_t y, char const *str, int mode);
void casio_Bdisp_DrawLine_VRAM(int x1, int y1, int x2, int y2);
void casio_RestoreDisp(unsigned char page);
void casio_SaveDisp(unsigned char page);
void *casio_Malloc(size_t size);
void *casio_Free(void *ptr);


// Internal casio abstraction.
static INLINE void dclear_area(int x1, int y1, int x2, int y2)
{
	struct rect area = {.left = x1, .top = y1, .right = x2, .bottom = y2};
	casio_Bdisp_AreaClr_VRAM(&area);
}
#define print(x, y, str)		casio_PrintMini(x, y, str, 0)
#define getkey				casio_GetKey
#define dline_horizontal(y, x1, x2)	casio_Bdisp_DrawLine_VRAM(x1, y, x2, y)
#define dclear				casio_Bdisp_AllClr_VRAM
#define save_window			casio_SaveDisp
#define restore_window			casio_RestoreDisp
#define malloc				casio_Malloc
#define free				casio_Free

#endif /*__CASIO_H__*/
