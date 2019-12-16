#ifndef __KERNEL_MODULES_SH7724_KEYSC_H__
# define __KERNEL_MODULES_SH7724_KEYSC_H__

#include <stdint.h>
#include <stddef.h>
#include <kernel/def/union_types.h>
#include <kernel/def/attributes.h>

struct sh7305_keysc_s
{
	volatile const uint16_t KIUDATA[6];
	volatile word_union(CONTROL,
		uint16_t KEYIE		: 1;	/* Key Interrupt enable */
		uint16_t const		:13;	/* All 0 */
		uint16_t INTMODE	: 2;	/* Key Interrupt mode (?) */
	);
	volatile word_union(AUTOFIX,
		uint16_t ENABLE		: 1;	/* enable AUTOFIX (?) */
		uint16_t const		: 8;	/* All 0 */
		uint16_t UNKNOWN0	: 3;	/* ????? */
		uint16_t const		: 1;	/* All 0 */
		uint16_t UNKNOWN1	: 3;	/* ????? */
	);
	volatile word_union(SCAN_MODE,
		uint16_t const		: 4;	/* All 0 */
		uint16_t UNKNOWN	: 3;	/* Scan mode (?) */
		uint16_t const		: 9;	/* All 0 */
	);
	volatile word_union(SCAN_STATE,
		uint16_t const		:15;	/* All 0 */
		uint16_t STATE		: 1;	/* ????? */
	);
	volatile word_union(INTERRUPT,
		uint16_t const		: 1;	/* All 0 */
		uint16_t KYCPU_IE	: 7;	/* KEYSC interrupt setup (see Planet Casio's bible) */
		uint16_t const STATUS	: 8;	/* Indicate the "type" of interruption (pressed, released, etc) */
	);
	volatile word_union(SCAN_WAIT,
		uint16_t const		: 1;	/* All 0 */
		uint16_t TIME		:15;	/* Time between interrupt */
	);
	volatile uint16_t SCAN_INTERVAL;
	volatile word_union(KYOUTDR,
		uint16_t const		: 4;	/* All 0 */
		uint16_t KYO5DT		: 2;	/* Key Controll ouput 5 */
		uint16_t KYO4DT		: 2;	/* Key Controll ouput 4 */
		uint16_t KYO3DT		: 2;	/* Key Controll ouput 3 */
		uint16_t KYO2DT		: 2;	/* Key Controll ouput 2 */
		uint16_t KYO1DT		: 2;	/* Key Controll ouput 1 */
		uint16_t KYO0DT		: 2;	/* Key Controll ouput 0 */
	);
	volatile word_union(KYINDR,
		uint16_t const		: 8;	/* All 0 */
		uint16_t KYDIR6		: 1;	/* Key Direction 6 */
		uint16_t KYDIR5		: 1;	/* Key Direction 5 */
		uint16_t KYDIR4		: 1;	/* Key Direction 4 */
		uint16_t KYDIR3		: 1;	/* Key Direction 3 */
		uint16_t KYDIR2		: 1;	/* Key Direction 2 */
		uint16_t KYDIR1		: 1;	/* Key Direction 1 */
		uint16_t KYDIR0		: 1;	/* Key Direction 0 */
	);
};

#define SH7305_KEYSC	(*(volatile struct sh7305_keysc_s *)0xa44b0000)

#endif /*__KERNEL_MODULES_SH7724_KEYSC_H__*/
