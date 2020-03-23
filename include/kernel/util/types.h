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

// Define MPU arch.
typedef enum mpu_e
{
	MPU_SH7305,
	MPU_SH7724,
	MPU_SH7337,
	MPU_SH7355,
	MPU_UNKNOWN,
} mpu_t;

typedef int32_t pid_t;
typedef int16_t mode_t;
typedef uint16_t dev_t;
typedef uint16_t umode_t;

#endif /*__TYPES_H__*/
