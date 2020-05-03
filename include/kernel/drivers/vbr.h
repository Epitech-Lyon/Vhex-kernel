#ifndef __KERNEL_DRIVERS_VBR_H__
# define __KERNEL_DRIVERS_VBR_H__

//---
//	General exception code
//---
#define EXP_CODE_USER_BREAK	0x1e0 // <- DBR (or VBR + 0x100)
#define EXP_CODE_ACCESS_READ	0x0e0
#define EXP_CODE_TLB_READ	0x040 // <- VBR + 0x400
#define EXP_CODE_TLB_WRITE	0x060
#define EXP_CODE_TLB_INITWRITE	0x080
#define EXP_CODE_TLB_PROTECT_R	0x0a0
#define EXP_CODE_TLB_PROTECT_W	0x0c0
#define EXP_CODE_ACCESS_WRITE	0x100
#define EXP_CODE_TRAPA		0x160
#define EXP_CODE_BAD_INSTR	0x180
#define EXP_CODE_BAD_SLOTINSTR	0x1a0

//---
//	Interruption code
//---
//TODO: chech SH7724 code validity (?)
#define INT_CODE_TMU0_TUNI0	0x400
#define INT_CODE_TMU0_TUNI1	0x420
#define INT_CODE_TMU0_TUNI2	0x440
#define INT_CODE_KEYSC		0xbe0

// Macros helpers
// @note:
// First interrupt code start at 0x400 (TLB miss are redirect)
// and the last interrupt code is 0xfe0 (undocumented)
// Each interrupt code are gapped by 0x20
#define VBR_GET_INTERRUPT_CODE(id, code)	(id = ((code - 0x400) / 0x20))
#define VBR_INTERRUPT_NB			((0xfe0 - 0x400) / 0x20)

// Driver primitives
extern void *vbr_interrupt_set(int intcode, void (*handler)(void));
extern void *vbr_set(void *vbr);
extern void *vbr_get(void);

#endif /*__KERNEL_DRIVERS_VBR_H__*/
