#ifndef __KERNEL_HARDWARE_SH7305_UBC_H__
# define __KERNEL_HARDWARE_SH7305_UBC_H__

#include <stdint.h>
#include <stddef.h>
#include <kernel/def/union_types.h>
#include <kernel/def/attributes.h>

struct sh7305_ubc_s
{
	//---
	//	Channel 0
	//---
	volatile long_union(CBR0,
		uint32_t MFE	: 1;	/* Match Flag Enable */
		uint32_t AIE	: 1;	/* ASID Enable */
		uint32_t MFI	: 6;	/* Match Flag Specify */
		uint32_t AIV	: 8;	/* ASID Specify */
		uint32_t const	: 1;	/* All 0 */
		uint32_t SZ	: 3;	/* Operend Size Select */
		uint32_t const	: 4;	/* All 0 */
		uint32_t CD	: 2;	/* Bus Select */
		uint32_t ID	: 2;	/* Instruction Fetch / Operand Access Select */
		uint32_t const	: 1;	/* All 0 */
		uint32_t RW	: 2;	/* Bus Command Select */
		uint32_t CE	: 1;	/* Channel Enable */
	);
	volatile long_union(CRR0,
		uint32_t const	: 18;	/* All 0 */
		uint32_t const	: 1;	/* All 1 */
		uint32_t const	: 11;	/* All 0 */
		uint32_t PCB	: 1;	/* PC Breack Select */
		uint32_t BIE	: 1;	/* Breack Enable */
	);
	volatile uint32_t CAR0;		/* Compare Address */
	volatile uint32_t CAMR0;	/* Compare Address Mask */
	GAPS(0x10);
	
	//---
	//	Channel 1
	//---
	volatile long_union(CBR1,
		uint32_t MFE	: 1;	/* Match Flag Enable */
		uint32_t AIE	: 1;	/* ASID Enable */
		uint32_t MFI	: 6;	/* Match Flag Specify */
		uint32_t AIV	: 8;	/* ASID Specify */
		uint32_t DBE	: 1;	/* Data Value Enable */
		uint32_t SZ	: 3;	/* Operend Size Select */
		uint32_t ETBE	: 1;	/* Execution Count Value Enable */
		uint32_t const	: 3;	/* All 0 */
		uint32_t CD	: 2;	/* Bus Select */
		uint32_t ID	: 2;	/* Instruction Fetch / Operand Access Select */
		uint32_t const	: 1;	/* All 0 */
		uint32_t RW	: 2;	/* Bus Command Select */
		uint32_t CE	: 1;	/* Channel Enable */
	);
	volatile long_union(CRR1,
		uint32_t const	: 18;	/* All 0 */
		uint32_t const	: 1;	/* All 1 */
		uint32_t const	: 11;	/* All 0 */
		uint32_t PCB	: 1;	/* PC Breack Select */
		uint32_t BIE	: 1;	/* Breack Enable */
	);
	volatile uint32_t CAR1;		/* Compare Address */
	volatile uint32_t CAMR1;	/* Compare Address Mask */

	//--
	//	Compare Data Part.
	//---
	volatile uint32_t CDR1;		/* Compare Data Value */
	volatile uint32_t CDMR1;	/* Compare Data Value Mask */


	//---
	//	Execution Count Register
	//---
	volatile long_union(CETR1,
		uint32_t const 	: 20;	/* All 0 */
		uint32_t CET	: 12;	/* Execution Count */
	);
	GAPS(0x5c4);

	//---
	//	Channel Match Flag Register
	//---
	volatile long_union(CCMFR,
		uint32_t const	: 30;	/* All 0 */
		uint32_t MF1	: 1;	/* Channel 1 Condition Match Flag */
		uint32_t MF0	: 1;	/* Channel 0 Condition Match Flag */
	);
	GAPS(0x1c);

	//---
	//	Control Register
	//---
	volatile long_union(CBCR,
		uint32_t const	: 31;	/* All 0 */
		uint32_t UBDE	: 1;	/* User Break Debugging Support Function Enable */
	);
};

#define SH7305_UBC	(*(volatile struct sh7305_ubc_s *)0xff200000)
#endif /*__KERNEL_HARDWARE_SH7305_UBC_H__*/
