#ifndef __FILE_H__
# define __FILE_H__

#include <stdint.h>
#include <stddef.h>
#include <kernel/types.h>

#ifndef FILE_OPEN_NUMBER
# define FILE_OPEN_NUMBER   4
#endif

struct file_s
{
    void *abstract;
    off_t cursor;
    uint8_t mode;
    size_t size;
};
typedef struct file_s FILE;

#endif /*__FILE_H__*/
