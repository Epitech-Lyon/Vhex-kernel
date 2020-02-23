#ifndef __FILE_H__
# define __FILE_H__

#include <stdint.h>
#include <stddef.h>
#include <kernel/util/types.h>
#include <kernel/fs/filesystem.h>

#ifndef FILE_OPEN_NUMBER
# define FILE_OPEN_NUMBER   4
#endif

#define O_RDONLY	0x00
#define O_WRONLY	0x01
#define O_RDWR		0x02

#define SEEK_SET	0
#define SEEK_CUR	1
#define SEEK_END	2

struct file_s
{
    void *private;
    off_t cursor;
    int permission;
    struct file_operations *file_op;
};
typedef struct file_s FILE;

#endif /*__FILE_H__*/
