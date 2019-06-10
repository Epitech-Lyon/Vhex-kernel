#ifndef __TYPES_H__
# define __TYPES_H__

#include <stddef.h>
#include <stdint.h>

//TODO: write doc

# ifndef __off_t_defined
typedef uint32_t off_t;
#  define __off_t_defined
# endif

#ifndef	__ssize_t_defined
typedef int32_t ssize_t;
# define __ssize_t_defined
#endif

#define INLINE	__attribute__((always_inline)) inline

#endif /*__TYPES_H__*/
