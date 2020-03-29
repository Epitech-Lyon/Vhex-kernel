#ifndef __FILE_H__
# define __FILE_H__

#include <stdint.h>
#include <stddef.h>
#include <asm/types.h>
#include <kernel/fs/filesystem.h>

#ifndef FILE_OPEN_NUMBER
# define FILE_OPEN_NUMBER   4
#endif

// FILE type
struct file_s
{
    void *private;
    off_t cursor;
    int permission;
    struct file_operations *file_op;
};
typedef struct file_s FILE;

#endif /*__FILE_H__*/
