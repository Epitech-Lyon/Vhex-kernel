#include <kernel/devices/tty.h>
#include <kernel/devices/device.h>

DEVICE(tty) = {
	.major = dev_make_major(TTY_DEV_MAJOR),
	.open = &tty_open,
	.file_op = {
		.read = (void *)&tty_read,
		.write =(void *)&tty_write,
		.ioctl = &tty_ioctl
	},
	.close = &tty_close
};
