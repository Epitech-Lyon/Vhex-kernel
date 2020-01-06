#ifndef __KERNEL_DEVICES_DEVICE_H__
# define __KERNEL_DEVICES_DEVICE_H__

#include <stddef.h>
#include <stdint.h>
#include <kernel/fs/filesystem.h>

#define dev_get_major(dev)	(dev & 0xff00)
#define dev_get_minor(dev)	(dev & 0x00ff)
#define dev_make_major(major)	((major & 0xff) << 8)

#define DEVICE(name)				\
	__attribute__((section(".device")))	\
	static struct device name##_dev

// Define dev_t
typedef uint16_t dev_t;

struct device
{
	dev_t major;
	void *(*open)(dev_t major, dev_t minor);
	struct file_operations file_op;
	int (*close)(void *inode);
};

#endif /*__KERNEL_DEVICES_DEVICE_H__*/
