#ifndef __KERNEL_HARDWARE_INTC_H__
# define __KERNEL_HARDWARE_INTC_H__

#include <stdint.h>
#include <stddef.h>
#include <kernel/bits/union_types.h>
#include <kernel/bits/attributes.h>

struct __sh7305_intc_s
{
	//---
	// Interrupt priority register.
	//---
	volatile word_union(IPRA,
		uint8_t TMU0_0	: 4;	/* Timer 0 */
		uint8_t TMU0_1	: 4;	/* Timer 1 */
		uint8_t TMU0_2	: 4;	/* Timer 2 */
		uint8_t IrDA	: 4;	/* Infrared Communication */
	);
	GAPS(2);
	volatile word_union(IPRB,
		uint8_t const	: 4;
		uint8_t LCDC	: 4;	/* LCD Controller */
		uint8_t DMAC1A	: 4;	/* Direct Memory Controller 1 */
		uint8_t const	: 4;
	);
	GAPS(2);
	volatile word_union(IPRC,
		uint8_t TMU1_0	: 4;	/* TMU1 Channel 0 */
		uint8_t TMU1_1	: 4;	/* TMU1 Channel 1 */
		uint8_t TMU1_2	: 4;	/* TMU1 Channel 2 */
		uint8_t const	: 4;
	);
	GAPS(2);
	volatile word_union(IPRD,
		uint8_t const	: 4;
		uint8_t MMCIF	: 4;	/* MultiMedia Card Interface */
		uint8_t const	: 4;
		uint8_t const	: 4;
	);
	GAPS(2);
	volatile word_union(IPRE,
		uint8_t DMAC0A	: 4;	/* Direct Memory Controller 1 */
		uint8_t const	: 4;
		uint8_t const	: 4;	/* Extra TMU ?? */
		uint8_t const	: 4;
	);
	GAPS(2);
	volatile word_union(IPRF,
		uint8_t KEYSC	: 4;	/* Key Scan Interface */
		uint8_t DMAC0B	: 4;	/* Direct Memory transfer / error info */
		uint8_t USB0_1	: 4;	/* USB Controller */
		uint8_t CMT	: 4;	/* Compare Match Timer*/
	);
	GAPS(2);
	volatile word_union(IPRG,
		uint8_t SCIF0	: 4;	/* SCIF0 transfert / error info */
		uint8_t const	: 4;	/* Extra TMU ?? */
		uint8_t const	: 4;	/* Extra TMU ?? */
		uint8_t const	: 4;
	);
	GAPS(2);
	volatile word_union(IPRH,
		uint8_t MSIOF0	: 4;	/* Clock-synchronized SCIF channel 0 */
		uint8_t MSIOF1	: 4;	/* Clock-synchronized SCIF channel 1 */
		uint8_t const	: 4;
		uint8_t const	: 4;
	);
	GAPS(2);
	volatile word_union(IPRI,
		uint8_t const	: 4;	/* Extra TMU ?? */
		uint8_t const	: 4;
		uint8_t const	: 4;
		uint8_t const	: 4;
	);
	GAPS(2);
	volatile word_union(IPRJ,
		uint8_t const	: 4;	/* Extra TMU ?? */
		uint8_t const	: 4;
		uint8_t FSI	: 4;	/* FIFO - buffered Serial Interface */
		uint8_t SDHI1	: 4;	/* SD Card Host channel 1 */
	);
	GAPS(2);
	volatile word_union(IPRK,
		uint8_t RTC	: 4;	/* Real Time Clock */
		uint8_t DMAC1B	: 4;	/* DMAC1 transfer/error info */
		uint8_t const	: 4;
		uint8_t SDHI0	: 4;	/* SD Card Host channel 0 */
	);
	GAPS(2);
	volatile word_union(IPRL,
		uint8_t const	: 4;	/* Extra TMU ?? */
		uint8_t const	: 4;
		uint8_t TPU	: 4;	/* Timer-Pulse Unit */
		uint8_t const	: 4;
	);
	GAPS(0x52);


	//---
	// Interrupt mask register.
	// FIXME
	// This is the SH7224 Interrupt mask registers, so find
	// the interrupt mask regiters for the SH7305.
	//---
	volatile byte_union(IMR0,
		uint8_t const	: 1;
		uint8_t TUNI2	: 1;	/* TMU1 (timer2) */
		uint8_t TUNI1	: 1;	/* TMU1 (timer1) */
		uint8_t TUNI0	: 1;	/* TMU1 (timer0) */
		uint8_t SDHII3	: 1;	/* SDHI1 */
		uint8_t SDHII2	: 1;	/* SDHI1 */
		uint8_t SDHII1	: 1;	/* SDHI1 */
		uint8_t SDHII0	: 1;	/* SDHI1 */
	);
	GAPS(3);
	volatile byte_union(IMR1,
		uint8_t VOUI	: 1;	/* VIO */
		uint8_t VEU1I	: 1;	/* VIO */
		uint8_t BEU0I	: 1;	/* VIO */
		uint8_t CEU0I	: 1;	/* VIO */
		uint8_t DEI3	: 1;	/* DMAC0A */
		uint8_t DEI2	: 1;	/* DMAC0A */
		uint8_t DEI1	: 1;	/* DMAC0A */
		uint8_t DEI0	: 1;	/* DMAC0A */
	);
	GAPS(3);
	volatile byte_union(IMR2,
		uint8_t const	: 1;
		uint8_t const	: 1;
		uint8_t const	: 1;
		uint8_t VPUI	: 1;	/* VPU */
		uint8_t ATAPI	: 1;	/* ATAPI */
		uint8_t EtherMAC: 1;	/* Ether */
		uint8_t const	: 1;
		uint8_t SCIFA0	: 1;	/* SCIFA0 */
	);
	GAPS(3);
	volatile byte_union(IMR3,
		uint8_t DEI3	: 1;	/* DMAC1A */
		uint8_t DEI2	: 1;	/* DMAC1A */
		uint8_t DEI1	: 1;	/* DMAC1A */
		uint8_t DEI0	: 1;	/* DMAC1A */
		uint8_t const	: 1;
		uint8_t const	: 1;
		uint8_t const	: 1;
		uint8_t IRDAI	: 1;	/* IrDA */
	);
	GAPS(3);
	volatile byte_union(IMR4,
		uint8_t const	: 1;
		uint8_t TUNI2	: 1;	/* TMU0 (timer2) */
		uint8_t TUNI1	: 1;	/* TMU0 (timer1) */
		uint8_t TUNI0	: 1;	/* TMU0 (timer0) */
		uint8_t JPUI	: 1;	/* JPU */
		uint8_t const	: 1;
		uint8_t const	: 1;
		uint8_t LCDCI	: 1;	/* LCDC */
	);
	GAPS(3);
	volatile byte_union(IMR5,
		uint8_t KEYI	: 1;	/* KEYSC */
		uint8_t DADERR	: 1;	/* DMAC0B */
		uint8_t DEI5	: 1;	/* DMAC0B */
		uint8_t DEI4	: 1;	/* DMAC0B */
		uint8_t VEU0I	: 1;	/* VEU3F0 */
		uint8_t SCIF2	: 1;	/* SCIF */
		uint8_t SCIF1	: 1;	/* SCIF */
		uint8_t SCIF0	: 1;	/* SCIF */
	);
	GAPS(3);
	volatile byte_union(IMR6,
		uint8_t const	: 1;
		uint8_t const	: 1;
		uint8_t ICBI	: 1;	/* ICB */
		uint8_t SCIFA4	: 1;	/* SCIFA4 */
		uint8_t CEU1I	: 1;	/* CEU1 */
		uint8_t const	: 1;
		uint8_t MSIOFI1	: 1;	/* MSIOF */
		uint8_t MSIOFI0	: 1;	/* MSIOF */
	);
	GAPS(3);
	volatile byte_union(IMR7,
		uint8_t DTE0I	: 1;	/* I2C0 */
		uint8_t WAITT0I	: 1;	/* I2C0 */
		uint8_t TACK0I	: 1;	/* I2C0 */
		uint8_t AL0I	: 1;	/* I2C0 */
		uint8_t DTE1I	: 1;	/* I2C1 */
		uint8_t WAITT1I	: 1;	/* I2C1 */
		uint8_t TACK1I	: 1;	/* I2C1 */
		uint8_t AL1I	: 1;	/* I2C1 */
	);
	GAPS(3);
	volatile byte_union(IMR8,
		uint8_t SDHII3	: 1;	/* SDHI0 */
		uint8_t SDHII2	: 1;	/* SDHI0 */
		uint8_t SDHII1	: 1;	/* SDHI0 */
		uint8_t SDHII0	: 1;	/* SDHI0 */
		uint8_t const	: 1;
		uint8_t const	: 1;
		uint8_t SCIFA5	: 1;	/* SCIFA5 */
		uint8_t FSI	: 1;	/* FSI */
	);
	GAPS(3);
	volatile byte_union(IMR9,
		uint8_t const	: 1;
		uint8_t const	: 1;
		uint8_t const	: 1;
		uint8_t CMTI	: 1;	/* CMT */
		uint8_t const	: 1;
		uint8_t USI1	: 1;	/* USB */
		uint8_t USI0	: 1;	/* USB */
		uint8_t const	: 1;
	);
	GAPS(3);
	volatile byte_union(IMR10,
		uint8_t const	: 1;
		uint8_t DADERR	: 1;	/* DMAC1B */
		uint8_t DEI5	: 1;	/* DMAC1B */
		uint8_t DEI4	: 1;	/* DMAC1B */
		uint8_t const	: 1;
		uint8_t ATI	: 1;	/* RTC */
		uint8_t PRI	: 1;	/* RTC */
		uint8_t CUI	: 1;	/* RTC */
	);
	GAPS(3);
	volatile byte_union(IMR11,
		uint8_t BRK	: 1;	/* 2DG */
		uint8_t CEI	: 1;	/* 2DG */
		uint8_t INI	: 1;	/* 2DG */
		uint8_t TRI	: 1;	/* 2DG */
		uint8_t const	: 1;
		uint8_t TPUI	: 1;	/* TPU */
		uint8_t LMBI	: 1;	/* ICB */
		uint8_t TSIFI	: 1;	/* TSIF */
	);
	GAPS(3);
	volatile byte_union(IMR12,
		uint8_t const	: 1;
		uint8_t const	: 1;
		uint8_t const	: 1;
		uint8_t const	: 1;
		uint8_t const	: 1;
		uint8_t const	: 1;
		uint8_t const	: 1;
		uint8_t _2DDMAC	: 1;	/* 2DDMAC */
	);
	GAPS(15);

	
	//---
	// Interrupt clear mask register.
	// FIXME
	// This is the SH7224 Interrupt mask registers, so find
	// the interrupt mask regiters for the SH7305.
	//---
	volatile byte_union(IMCR0,
		uint8_t const	: 1;
		uint8_t TUNI2	: 1;	/* TMU1 (timer2) */
		uint8_t TUNI1	: 1;	/* TMU1 (timer1) */
		uint8_t TUNI0	: 1;	/* TMU1 (timer0) */
		uint8_t SDHII3	: 1;	/* SDHI1 */
		uint8_t SDHII2	: 1;	/* SDHI1 */
		uint8_t SDHII1	: 1;	/* SDHI1 */
		uint8_t SDHII0	: 1;	/* SDHI1 */
	);
	GAPS(3);
	volatile byte_union(IMCR1,
		uint8_t VOUI	: 1;	/* VIO */
		uint8_t VEU1I	: 1;	/* VIO */
		uint8_t BEU0I	: 1;	/* VIO */
		uint8_t CEU0I	: 1;	/* VIO */
		uint8_t DEI3	: 1;	/* DMAC0A */
		uint8_t DEI2	: 1;	/* DMAC0A */
		uint8_t DEI1	: 1;	/* DMAC0A */
		uint8_t DEI0	: 1;	/* DMAC0A */
	);
	GAPS(3);
	volatile byte_union(IMCR2,
		uint8_t const	: 1;
		uint8_t const	: 1;
		uint8_t const	: 1;
		uint8_t VPUI	: 1;	/* VPU */
		uint8_t ATAPI	: 1;	/* ATAPI */
		uint8_t EtherMAC: 1;	/* Ether */
		uint8_t const	: 1;
		uint8_t SCIFA0	: 1;	/* SCIFA0 */
	);
	GAPS(3);
	volatile byte_union(IMCR3,
		uint8_t DEI3	: 1;	/* DMAC1A */
		uint8_t DEI2	: 1;	/* DMAC1A */
		uint8_t DEI1	: 1;	/* DMAC1A */
		uint8_t DEI0	: 1;	/* DMAC1A */
		uint8_t const	: 1;
		uint8_t const	: 1;
		uint8_t const	: 1;
		uint8_t IRDAI	: 1;	/* IrDA */
	);
	GAPS(3);
	volatile byte_union(IMCR4,
		uint8_t const	: 1;
		uint8_t TUNI2	: 1;	/* TMU0 (timer2) */
		uint8_t TUNI1	: 1;	/* TMU0 (timer1) */
		uint8_t TUNI0	: 1;	/* TMU0 (timer0) */
		uint8_t JPUI	: 1;	/* JPU */
		uint8_t const	: 1;
		uint8_t const	: 1;
		uint8_t LCDCI	: 1;	/* LCDC */
	);
	GAPS(3);
	volatile byte_union(IMCR5,
		uint8_t KEYI	: 1;	/* KEYSC */
		uint8_t DADERR	: 1;	/* DMAC0B */
		uint8_t DEI5	: 1;	/* DMAC0B */
		uint8_t DEI4	: 1;	/* DMAC0B */
		uint8_t VEU0I	: 1;	/* VEU3F0 */
		uint8_t SCIF2	: 1;	/* SCIF */
		uint8_t SCIF1	: 1;	/* SCIF */
		uint8_t SCIF0	: 1;	/* SCIF */
	);
	GAPS(3);
	volatile byte_union(IMCR6,
		uint8_t const	: 1;
		uint8_t const	: 1;
		uint8_t ICBI	: 1;	/* ICB */
		uint8_t SCIFA4	: 1;	/* SCIFA4 */
		uint8_t CEU1I	: 1;	/* CEU1 */
		uint8_t const	: 1;
		uint8_t MSIOFI1	: 1;	/* MSIOF */
		uint8_t MSIOFI0	: 1;	/* MSIOF */
	);
	GAPS(3);
	volatile byte_union(IMCR7,
		uint8_t DTE0I	: 1;	/* I2C0 */
		uint8_t WAITT0I	: 1;	/* I2C0 */
		uint8_t TACK0I	: 1;	/* I2C0 */
		uint8_t AL0I	: 1;	/* I2C0 */
		uint8_t DTE1I	: 1;	/* I2C1 */
		uint8_t WAITT1I	: 1;	/* I2C1 */
		uint8_t TACK1I	: 1;	/* I2C1 */
		uint8_t AL1I	: 1;	/* I2C1 */
	);
	GAPS(3);
	volatile byte_union(IMCR8,
		uint8_t SDHII3	: 1;	/* SDHI0 */
		uint8_t SDHII2	: 1;	/* SDHI0 */
		uint8_t SDHII1	: 1;	/* SDHI0 */
		uint8_t SDHII0	: 1;	/* SDHI0 */
		uint8_t const	: 1;
		uint8_t const	: 1;
		uint8_t SCIFA5	: 1;	/* SCIFA5 */
		uint8_t FSI	: 1;	/* FSI */
	);
	GAPS(3);
	volatile byte_union(IMCR9,
		uint8_t const	: 1;
		uint8_t const	: 1;
		uint8_t const	: 1;
		uint8_t CMTI	: 1;	/* CMT */
		uint8_t const	: 1;
		uint8_t USI1	: 1;	/* USB */
		uint8_t USI0	: 1;	/* USB */
		uint8_t const	: 1;
	);
	GAPS(3);
	volatile byte_union(IMCR10,
		uint8_t const	: 1;
		uint8_t DADERR	: 1;	/* DMAC1B */
		uint8_t DEI5	: 1;	/* DMAC1B */
		uint8_t DEI4	: 1;	/* DMAC1B */
		uint8_t const	: 1;
		uint8_t ATI	: 1;	/* RTC */
		uint8_t PRI	: 1;	/* RTC */
		uint8_t CUI	: 1;	/* RTC */
	);
	GAPS(3);
	volatile byte_union(IMCR11,
		uint8_t BRK	: 1;	/* 2DG */
		uint8_t CEI	: 1;	/* 2DG */
		uint8_t INI	: 1;	/* 2DG */
		uint8_t TRI	: 1;	/* 2DG */
		uint8_t const	: 1;
		uint8_t TPUI	: 1;	/* TPU */
		uint8_t LMBI	: 1;	/* ICB */
		uint8_t TSIFI	: 1;	/* TSIF */
	);
	GAPS(3);
	volatile byte_union(IMCR12,
		uint8_t const	: 1;
		uint8_t const	: 1;
		uint8_t const	: 1;
		uint8_t const	: 1;
		uint8_t const	: 1;
		uint8_t const	: 1;
		uint8_t const	: 1;
		uint8_t _2DDMAC	: 1;	/* 2DDMAC */
	);
	GAPS(0xbff0e);

	//---
	//	Interrupt Control registers 0
	//---
	volatile word_union(ICR0,
		uint8_t NMIL	:1;	/* Non Maskable Interrupt Input level */
		uint8_t MAI	:1;	/* Non Maskable Interrupt interrupt Mask */
		uint8_t const	:4;	/* All 0 */
		uint8_t NMIB	:1;	/* Non Maskable Interrupt Block Mode */
		uint8_t NMIE	:1;	/* Non Maskable Interrupt Edge Select */
		uint8_t const	:2;	/* All 1 */
		uint8_t LVLMODE	:1;	/* Interrupt Source Retention Mode */
		uint8_t const	:5;	/* All 0 */
	);
	GAPS(0x0e);
	

	//---
	//	Interrupt Priority registers
	//---
	volatile long_union(INTPRI00,
		uint32_t IRQ0	: 4;	/* Interrupt Request 0 */
		uint32_t IRQ1	: 4;	/* Interrupt Request 1 */
		uint32_t IRQ2	: 4;	/* Interrupt Request 2 */
		uint32_t IRQ3	: 4;	/* Interrupt Request 3 */
		uint32_t IRQ4	: 4;	/* Interrupt Request 4 */
		uint32_t IRQ5	: 4;	/* Interrupt Request 5 */
		uint32_t IRQ6	: 4;	/* Interrupt Request 6 */
		uint32_t IRQ7	: 4;	/* Interrupt Request 7 */
	);
	GAPS(8);

	//---
	//	Interrupt Control register 1
	//---
	volatile word_union(ICR1,
		uint8_t IRQ0S	:2;	/* Interrupt Request 0 Sense Select */
		uint8_t IRQ1S	:2;	/* Interrupt Request 1 Sense Select */
		uint8_t IRQ2S	:2;	/* Interrupt Request 2 Sense Select */
		uint8_t IRQ3S	:2;	/* Interrupt Request 3 Sense Select */
		uint8_t IRQ4S	:2;	/* Interrupt Request 4 Sense Select */
		uint8_t IRQ5S	:2;	/* Interrupt Request 5 Sense Select */
		uint8_t IRQ6S	:2;	/* Interrupt Request 6 Sense Select */
		uint8_t IRQ7S	:2;	/* Interrupt Request 7 Sense Select */
	);
	GAPS(6);

	//---
	//	Interrupt request register.
	//---
	volatile byte_union(INTREQ00,
		uint8_t IRQ0	: 1;	/* Interrupt Request 0 */
		uint8_t IRQ1	: 1;	/* Interrupt Request 1 */
		uint8_t IRQ2	: 1;	/* Interrupt Request 2 */
		uint8_t IRQ3	: 1;	/* Interrupt Request 3 */
		uint8_t IRQ4	: 1;	/* Interrupt Request 4 */
		uint8_t IRQ5	: 1;	/* Interrupt Request 5 */
		uint8_t IRQ6	: 1;	/* Interrupt Request 6 */
		uint8_t IRQ7	: 1;	/* Interrupt Request 7 */
	);
	GAPS(0x1f);

	//---
	//	Interrupt mask register.
	//---
	volatile byte_union(INTMSK00,
		uint8_t IRQ0	: 1;	/* Interrupt Request 0 */
		uint8_t IRQ1	: 1;	/* Interrupt Request 1 */
		uint8_t IRQ2	: 1;	/* Interrupt Request 2 */
		uint8_t IRQ3	: 1;	/* Interrupt Request 3 */
		uint8_t IRQ4	: 1;	/* Interrupt Request 4 */
		uint8_t IRQ5	: 1;	/* Interrupt Request 5 */
		uint8_t IRQ6	: 1;	/* Interrupt Request 6 */
		uint8_t IRQ7	: 1;	/* Interrupt Request 7 */
	);
	GAPS(0x1f);

	//---
	//	Interrupt mask clear register. (write only).
	//---
	volatile byte_union(INTMSKCLR00,
		uint8_t IRQ0	: 1;	/* Interrupt Request 0 */
		uint8_t IRQ1	: 1;	/* Interrupt Request 1 */
		uint8_t IRQ2	: 1;	/* Interrupt Request 2 */
		uint8_t IRQ3	: 1;	/* Interrupt Request 3 */
		uint8_t IRQ4	: 1;	/* Interrupt Request 4 */
		uint8_t IRQ5	: 1;	/* Interrupt Request 5 */
		uint8_t IRQ6	: 1;	/* Interrupt Request 6 */
		uint8_t IRQ7	: 1;	/* Interrupt Request 7 */
	);
	GAPS(0x5b);

	//---
	//	Non Maskable Interrupt flags control register
	//---
	volatile word_union(NMIFCR,
		uint16_t const NMIL	: 1;	/* NMI input Level */
		uint16_t const		: 14;	/* All 0 */
		uint16_t NMIFL		: 1;	/* NMI Interrupt Request Detection */
	);
	GAPS(0x5bff3e);

	//---
	//	User interrupt mask level register.
	//---
	volatile long_union(USERMSK,
		uint32_t SECRET	: 8;	/* Only write 0xa5 */
		uint32_t const	: 8;	/* All 0 */
		uint32_t UIMASK	: 4;	/* User Interrupt Mask Level */
		uint32_t const	: 4;	/* All 0 */
	);
};

struct __sh7305_intc_context
{
	uint16_t ipra;
	uint16_t iprb;
	uint16_t iprc;
	uint16_t iprd;
	uint16_t ipre;
	uint16_t iprf;
	uint16_t iprg;
	uint16_t iprh;
	uint16_t ipri;
	uint16_t iprj;
	uint16_t iprk;
	uint16_t iprl;
	uint8_t imr[12];
	uint16_t icr[2];
	uint32_t intpri00;
	uint8_t intreq00;
	uint8_t intmsk00;
	uint8_t intmskclr00;
	uint16_t nmifcr;
	uint32_t usermsk;
} __attribute__((packed, aligned(2)));

#define SH7305_INTC	(*(volatile struct __sh7305_intc_s	*)0xa4080000)
#endif /*__KERNEL_HARDWARE_INTC_H__*/
