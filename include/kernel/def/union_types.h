#ifndef __KERNEL_MODULES_TYPES_H__
# define __KERNEL_MODULES_TYPES_H__

#include <stdint.h>
#include <stddef.h>

#define byte_union(name, fields)			\
	union {						\
		uint8_t BYTE;				\
		struct {				\
			fields				\
		} __attribute__((packed, aligned(1)));	\
	} __attribute__((packed, aligned(1))) name

#define word_union(name, fields)			\
	union {						\
		uint16_t WORD;				\
		struct {				\
			fields				\
		} __attribute__((packed, aligned(2)));	\
	} __attribute__((packed, aligned(2))) name

#define long_union(name, fields)			\
	union {						\
		uint32_t LONG_WORD;			\
		struct {				\
			fields				\
		} __attribute__((packed, aligned(4)));	\
	} __attribute__((packed, aligned(4))) name


#endif /*__KERNEL_MODULES_TYPES_H__*/
