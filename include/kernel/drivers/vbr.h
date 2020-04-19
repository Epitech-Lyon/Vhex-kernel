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
//TODO


#endif /*__KERNEL_DRIVERS_VBR_H__*/
