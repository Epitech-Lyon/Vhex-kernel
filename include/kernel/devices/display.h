#ifndef __KERNEL_DEVICES_DISPLAY_H__
# define __KERNEL_DEVICES_DISPLAY_H__

#include <stddef.h>
#include <stdint.h>
#include <kernel/util/types.h>
#include <kernel/util/draw.h>

// Define screen informations.
#define DISPLAY_SCREEN_WIDTH		(128)
#define DISPLAY_SCREEN_HEIGHT		(64)

// Define Number of vertical lines
// and horizontal lines.
#define DISPLAY_VCHAR_MAX		(DISPLAY_SCREEN_HEIGHT / (KERNEL_FONT_REAL_HEIGHT + 1))
#define DISPLAY_HCHAR_MAX		(DISPLAY_SCREEN_WIDTH / (KERNEL_FONT_REAL_WIDTH + 1))

// Device primitives.
extern int display_open(void);
extern ssize_t display_write(const void *buffer, size_t count);
extern int display_close(void);

// IOCTL code.
#define DISPLAY_IOCTL_GETX	(0x00)
#define DISPLAY_IOCTL_GETY	(0x01)
#define DISPLAY_IOCTL_SETX	(0x02)
#define DISPLAY_IOCTL_SETY	(0x03)
#define DISPLAY_IOCTL_CLEAR	(0x04)
#define DISPLAY_IOCTL_DISPLAY	(0x05)
extern void display_ioctl(uint32_t cmd, ...);


// Sheared Video RAM
// @note:
// 	The user *SHOULD* not use the
// VRAM without atomic operation (because
// of reentrance).
extern uint32_t vram[256];

#endif /*__KERNEL_DEVICES_DISPLAY_H__*/
