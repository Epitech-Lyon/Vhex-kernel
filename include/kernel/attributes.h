#ifndef __KERNEL_ARCH_ATTRIBUTES_H__
# define __KERNEL_ARCH_ATTRIBUTES_H__

// Generate "anonyme name" for each gaps.
#define gap_name2(name)	_##name
#define gap_name(name)	gap_name2(name)
#define GAPS(bytes)	const uint8_t gap_name(__LINE__)[bytes]

#endif /*__KERNEL_ARCH_ATTRIBUTES_H__*/
