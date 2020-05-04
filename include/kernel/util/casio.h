#ifndef __KERNEL_UTIL_CASIO_H__
# define __KERNEL_UTIL_CASIO_H__

#include <stddef.h>
#include <stdint.h>
#include <sys/types.h>

//---
//	Casio's wrapper
//---
extern void casio_return_menu(int mode);



// Internal Casio datat structure
struct rect
{
	int left;
	int top;
	int right;
	int bottom;
};

// GetKeyWait macros
#define KEYWAIT_HALTON_TIMEROFF 	0
#define KEYWAIT_HALTOFF_TIMEROFF	1
#define KEYWAIT_HALTON_TIMERON		2

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

// Timer syscalls
extern int casio_TimerInstall(int id, void (*handler)(void), int delay_ms);
extern int casio_TimerUninstall(int id);
extern int casio_TimerStart(int id);
extern int casio_TimerStop(int id);

//---
// Bfile syscalls
//---
//@note: Graph35+II define
// DT_ADDIN_APP -> 65
// DT_DIRECTORY -> 0
// DT_DOT	-> 8
// DT_DOT_DOT	-> 9
#define DT_DIRECTORY            0x0000      // Directory
#define DT_FILE                 0x0001      // File
#define DT_ADDIN_APP            0x0002      // Add-In application
#define DT_EACT                 0x0003      // eActivity
#define DT_LANGUAGE             0x0004      // Language
#define DT_BITMAP               0x0005      // Bitmap
#define DT_MAINMEM              0x0006      // Main Memory data
#define DT_TEMP                 0x0007      // Temporary data
#define DT_DOT                  0x0008      // .  (Current directory)
#define DT_DOTDOT               0x0009      // .. (Parent directory)
#define DT_VOLUME               0x000A      // Volume label

// @note: unsed with Graph35+
#define _OPENMODE_READ              0x01
#define _OPENMODE_READ_SHARE        0x80
#define _OPENMODE_WRITE             0x02
#define _OPENMODE_READWRITE         0x03
#define _OPENMODE_READWRITE_SHARE   0x83


// Internal Casio's structure to dump file's informations
struct casio_file_info
{
	uint16_t id;			// File index
	uint16_t type;			// File type
	struct {
		uint32_t file;		// File size (data + header)
		uint32_t data;		// Data size (without header)
	} size;
	uint32_t address;		// Data address ?
};
extern int casio_Bfile_OpenFile(uint16_t *pathname, int mode);
extern int casio_Bfile_ReadFile(int handle, void *buffer, size_t count, off_t pos);
extern int casio_Bfile_FindFirst(uint16_t *search_path, int *handle,
		uint16_t *pathname, struct casio_file_info *file_info);
extern int casio_Bfile_FindNext(int handle, uint16_t *pathname, struct casio_file_info *file_info);
extern int casio_Bfile_FindClose(int handle);
extern int casio_Bfile_CloseFile(int handle);


#endif /*__KERNEL_UTIL_CASIO_H__*/
