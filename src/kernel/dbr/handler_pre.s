.section ".ubc.handler", "awx", @progbits

.global	_ubc_handler_pre
.type	_ubc_handler_pre, @function

.extern	_ubc_handler
.extern	_ubc_module_handler


.align 2
_ubc_handler_pre:
	! Stack management.
	mov.l	r9, @-r15		! Save r9 register.
	mov.l	r8, @-r15		! Save r8 register.
	sts.l	pr, @-r15		! Save pr regsiter.

	! Generate "programe" context (used by the UBC handler)
	stc.l	spc, @-r15		! Get SPC register.
	stc.l	ssr, @-r15		! Get SSR register.
	sts.l	mach, @-r15		! Get MACH register.
	sts.l	macl, @-r15		! Get MACL register.
	stc.l	gbr, @-r15		! Get GBR register.
	.word	0b0100111100110010	! Get SGR register (save r15 address befor break) "stc.l sgr, @-r15"
	mov.l	r14, @-r15		! Get "program" r14 register.
	mov.l	r13, @-r15		! Get "program" r13 register.
	mov.l	r12, @-r15		! Get "program" r12 register.
	mov.l	r11, @-r15		! Get "program" r11 register.
	mov.l	r10, @-r15		! Get "program" r10 register.
	mov.l	r9, @-r15		! Get "program" r9 register.
	mov.l	r8, @-r15		! Get "program" r8 register.
	stc.l	R7_BANK, @-r15		! Get "program" r7 regsiter.
	stc.l	R6_BANK, @-r15		! Get "program" r6 regsiter.
	stc.l	R5_BANK, @-r15		! Get "program" r5 regsiter.
	stc.l	R4_BANK, @-r15		! Get "program" r4 regsiter.
	stc.l	R3_BANK, @-r15		! Get "program" r3 regsiter.
	stc.l	R2_BANK, @-r15		! Get "program" r2 regsiter.
	stc.l	R1_BANK, @-r15		! Get "program" r1 regsiter.
	stc.l	R0_BANK, @-r15		! Get "program" r0 regsiter.

	! We need to stop (and save) all clocks / timers
	! before do any job.
	mov.l	.ubc_module_handler, r0	! Get high level abstraction for handle hardware module.
	jsr	@r0			! call ubc_module(MODULE_STOP)
	mov	#0, r4			! (db) r4 = MODULE_STOP

	! Get which channel is trigger and clear interrupt Flags.
	mov.l	.ubc_ccmfr, r0		! Get UBC.CCMFR register
	mov.l	@r0, r9			! r5 = UBC.CCMFR. (save register)
	mov	#0, r1			! r2 = 0x00000000 (clear flags)
	mov.l	r1, @r0			! Clear UBC.CCMFR.MF1 = 0 and UBC.CCMFR.MF1 = 0
	mov.l	.icbi_addr, r2		! Get P2 area for ICBI instruction.
	.word	0b0000001011100011	! SH4 instruction "icbi @r2"

	! Allow / unblock interrupt and switch register bank !
	stc	sr, r8			! Save SR register.
	mov	r8, r1			! Get SR register.
	mov.l	.sr_mask, r0		! Get SR mask for SR.BL, SR.IMASK and SR.RB
	and	r0, r1			! SR.BL = 0, SR.IMASK = 0b0000 and SR.RB = 0
	ldc	r1, sr			! Update SR register.

	! Call high-level abstraction
	mov	r15, r4			! Send UBC context object to the abstraction.
	mov	r9, r5			! Send which channel is trigger.
	mov.l	.ubc_handler, r0	! Get high-level abstraction address
	jsr	@r0			! Jump into it.
	nop				! (db) nop.

	! Block interrupt and switch
	! register bank
	ldc	r8, sr			! Restore SR register (with SR.BL = 1, SR.IMASK = 0b1111, SR.RB = 1)

	! Restore "program" context.
	ldc.l	@r15+, R0_BANK		! Restore "program" r0 regsiter.
	ldc.l	@r15+, R1_BANK		! Restore "program" r1 regsiter.
	ldc.l	@r15+, R2_BANK		! Restore "program" r2 regsiter.
	ldc.l	@r15+, R3_BANK		! Restore "program" r3 regsiter.
	ldc.l	@r15+, R4_BANK		! Restore "program" r4 regsiter.
	ldc.l	@r15+, R5_BANK		! Restore "program" r5 regsiter.
	ldc.l	@r15+, R6_BANK		! Restore "program" r6 regsiter.
	ldc.l	@r15+, R7_BANK		! Restore "program" r7 regsiter.
	mov.l	@r15+, r8		! Restore "program" r8 regsiter.
	mov.l	@r15+, r9		! Restore "program" r9 regsiter.
	mov.l	@r15+, r10		! Restore "program" r10 regsiter.
	mov.l	@r15+, r11		! Restore "program" r11 regsiter.
	mov.l	@r15+, r12		! Restore "program" r12 regsiter.
	mov.l	@r15+, r13		! Restore "program" r13 regsiter.
	mov.l	@r15+, r14		! Restore "program" r14 regsiter.
	.word	0b0100111100110110	! Restore SGR regsiter. "ldc.l @r15+, sgr"
	ldc.l	@r15+, gbr		! Get GBR register.
	lds.l	@r15+, macl		! Get MACL register.
	lds.l	@r15+, mach		! Get MACH register.
	ldc.l	@r15+, ssr		! Restore SSR regsiter.
	ldc.l	@r15+, spc		! Restore SPC regsiter.

	! Retore and restart clock / timers
	stc	spc, r4			! (db) r4 = MODULE_START
	mov.l	.ubc_module_handler, r0	! Get high level abstraction for handle hardware module.
	jsr	@r0			! call ubc_module(MODULE_START)
	nop

	! Clean exit.
	lds.l	@r15+, pr		! Restor PR register.
	mov.l	@r15+, r8		! Restore r8 register.
	mov.l	@r15+, r9		! Restore r8 register.
	rte				! Interrupt Exit.
	nop				! (db) Safety first.


.align 4
.ubc_handler:		.long _ubc_handler
.ubc_module_handler:	.long _ubc_module_handler
.ubc_ccmfr:		.long 0xff200600
.icbi_addr:		.long 0xa0000000
.sr_mask:		.long ~(0x300000f0)
.end
