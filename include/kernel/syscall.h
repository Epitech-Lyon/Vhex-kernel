#ifndef __CASIO_H__
# define __CASIO_H__

#include <stdint.h>
#include <stddef.h>
#include <kernel/types.h>

//---
//
//	Vhex part !!
//
//---
extern pid_t sys_fork(void);
extern pid_t sys_getpid(void);
extern pid_t sys_getppid(void);
extern pid_t sys_waitpid(pid_t pid, int *wstatus, int options);

extern int sys_open(const char *pathname, int flags, ...);
extern ssize_t sys_write(int fd, const void *buf, size_t count);
extern ssize_t sys_read(int fd, void *buf, size_t count);
extern off_t sys_lseek(int fd, off_t offset, int whence);
extern int sys_close(int fd);

//---
//
//	CASIO PART !!
//	TODO: remove me ?
//
//----


// Internal Casio datat structure
struct rect
{
	int left;
	int top;
	int right;
	int bottom;
};

//
// Casio prototypes.
//
/* GetKey() - display Casio's VRAM and wait keyboard input. */
void casio_GetKey(unsigned int *key);
void casio_GetKeyWait(int *row, int *column, int type_waiting, int timeout, int menu, unsigned int *key);

/* Bdisp_PutDisp_DD - display Casio's VRAM on screen */
void casio_Bdisp_PutDisp_DD(void);

/* Bdisp_AllClr_VRAM() - clear entirely the Casio's VRAM */
void casio_Bdisp_AllClr_VRAM(void);

/* Bdisp_AreaClr_VRAM() - clear only VRAM area. */
void casio_Bdisp_AreaClr_VRAM(const struct rect *buf);

/* PrintMini() - print string in Casio's VRAM (and display on screen ?) */
void casio_Bdisp_PrintMini(size_t x, size_t y, char const *str, int mode);

/* Bdisp_DrawLine_VRAM() - draw line in Casio's VRAM. */
void casio_Bdisp_DrawLine_VRAM(int x1, int y1, int x2, int y2);

/* RestoreDisp() - restore saved screen. */
void casio_RestoreDisp(unsigned char page);

/* SaveDisp() - save the content of the screen. */
void casio_SaveDisp(unsigned char page);

/* Malloc() - malloc syscall */
void *casio_Malloc(size_t size);

/* Free() - free syscall */
void *casio_Free(void *ptr);

/* GetVRAM - Get the Video RAM used by Casio */
void *casio_Bdisp_GetVRAM(void);

/* Bkey_PutKeyCode - Inject keycode to key buffer */
void casio_Bkey_PutKeymatrix(uint16_t *code);

// Internal casio abstraction.
static inline void dclear_area(int x1, int y1, int x2, int y2)
{
	struct rect area = {.left = x1, .top = y1, .right = x2, .bottom = y2};
	casio_Bdisp_AreaClr_VRAM(&area);
}

#endif /*__CASIO_H__*/
