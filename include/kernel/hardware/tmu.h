#ifndef __KERNEL_ARCH_SH7305_TMU_H__
# define __KERNEL_ARCH_SH7305_TMU_H__

#include <stdint.h>
#include <stddef.h>
#include <kernel/def/union_types.h>
#include <kernel/def/attributes.h>

struct timer_s
{
	volatile uint32_t TCOR;         /* Timer Constant register */
	volatile uint32_t TCNT;         /* Timer Counter */
	volatile word_union(TCR,        /* Timer Control Register */
		uint16_t const  : 7;    /* All 0 */
		uint16_t UNF    : 1;    /* Underflow flags */
		uint16_t const  : 2;    /* All 0 */
		uint16_t UNIE   : 1;    /* Underflow interrup control */
		uint16_t const  : 2;    /* All 0 */
		uint16_t TPSC   : 3;    /* Time prescaler */
	);
	GAPS(2);
};

struct __sh7305_tmu_s
{
	volatile byte_union(TSTR,                /* Timer Start Register */
		uint8_t const   : 5;                /* All 0 */
		uint8_t STR2    : 1;                /* Counter start 2 */
		uint8_t STR1    : 1;                /* Counter start 1 */
		uint8_t STR0    : 1;                /* Counter start 0 */
	);
	GAPS(3);
	volatile struct timer_s TIMER[3];
};

#define SH7305_TMU (*(volatile struct __sh7305_tmu_s  *)0xa4490004)
#endif /*__KERNEL_ARCH_SH7305_TMU_H__*/
