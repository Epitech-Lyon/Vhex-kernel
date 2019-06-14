#ifndef __TYPES_H__
# define __TYPES_H__

#include <stddef.h>
#include <stdint.h>

// Define properly off_t type.
# ifndef __off_t_defined
typedef uint32_t off_t;
#  define __off_t_defined
# endif

// Define properly ssize_t type.
#ifndef	__ssize_t_defined
typedef int32_t ssize_t;
# define __ssize_t_defined
#endif

// Force inline function.
#define INLINE	__attribute__((always_inline)) inline

#endif /*__TYPES_H__*/
