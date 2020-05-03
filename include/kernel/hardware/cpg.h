#ifndef __KERNEL_HARDWARE_CPG_H__
# define __KERNEL_HARDWARE_CPG_H__

#include <stddef.h>
#include <stdint.h>
#include <kernel/bits/union_types.h>
#include <kernel/bits/attributes.h>

struct SH7305_cpg_s
{
	volatile long_union(FRQCRA,		/* Casio's configuration: 0x0f212213 */
		uint32_t KICK		: 1;	/* Kick bits */
		uint32_t const		: 1;	/* All 0 */
		uint32_t STC		: 6;	/* PLL circuit multiplication ratio */
		uint32_t IFC		: 4;	/* CPU clock frequency division ratio */
		uint32_t const		: 4;	/* All 0 */
		uint32_t SFC		: 4;	/* S clock frequency division ratio */
		uint32_t BFC		: 4;	/* Bus clock frequency division ration */
		uint32_t const		: 4;	/* All 0 */
		uint32_t PFC		: 4;	/* Peripheral clock frequency division ratio */
	);
	volatile uint32_t FRQCRB;		/* Undocumented (0x00000000) */
	volatile long_union(FCLKACR,		/* Casio's configuration: 0x00000157 */
		uint32_t const		: 16;	/* All 0 */
		uint32_t unknown	: 16;	/* Unknown (default 0x0157) */
	);
	volatile long_union(FCLKBCR,		/* Casio's configuration: 0x0000003f */
		uint32_t const		: 16;	/* All 0 */
		uint32_t unknown	: 16;	/* Unknown (default 0x003f) */
	);

	GAPS(0x08);
	volatile long_union(IRDACLKCR,		/* Casio's configuration: 0x0000003f */
		uint32_t const		: 16;	/* All 0 */
		uint32_t unknown	: 16;	/* Unknown (default 0x003f) */
	);

	GAPS(0x08);
	volatile long_union(PLLCR,		/* Casio's configuration: 0x00005000 */
		uint32_t const		: 16;	/* All 0 */
		uint32_t unknown0	: 1;	/* Unknown (default 0) */
		uint32_t PLLE		: 1;	/* PLL enable (?) */
		uint32_t const		: 1;	/* All 0 */
		uint32_t FLLE		: 1;	/* FLL enable (?) */
		uint32_t const		: 8;	/* All 0 */
		uint32_t unknown1	: 4;	/* Unknown (default 0b0000) */
	);

	GAPS(0x14);
	volatile long_union(SPUCLKCR,		/* Casio's configuration: 0x00000103 */
		uint32_t const		: 16;	/* All 0 */
		uint32_t unknown	: 16;	/* Unknown (default 0x0103) */
	);

	GAPS(0x08);
	volatile long_union(VCLKCR,		/* Casio's configuration: 0x0000003f */
		uint32_t const		: 16;	/* All 0 */
		uint32_t unknown	: 16;	/* Unknown (default 0x003f) */
	);


	GAPS(0x04);
	volatile long_union(FLLFRQ,		/* Casio's configuration: 0x00004384 */
		uint32_t const		: 16;	/* All 0 */
		uint32_t SELXM		: 2;    /* FLL output division */
		uint32_t unknown	: 3;	/* Unknown */
		uint32_t FLF		: 11;	/* FLL Multiplication Ratio */

	);

	GAPS(0x0c);
	volatile long_union(LSTATS,
		uint32_t const		: 31;	/* All 0 */
		uint32_t const FQRF	: 1;	/* Frequency change status */
	);
};


#define SH7305_CPG	(*(volatile struct SH7305_cpg_s	*)0xa4150000)
#endif /*__KERNEL_HARDWARE_CPG_H__*/
