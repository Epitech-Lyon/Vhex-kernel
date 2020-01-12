.text

.global	_kernel_switch
.type	_kernel_switch, @function

.align 2
_kernel_switch:
	! Save process context into unbakable register
	mov	r4, r8

	! Update SR register to block
	! interrupt / exception and
	! switch register bank to simalate
	! system call.
	mov.l	.sr_msk, r1	! get mask for SR.BL = 1, SR.RB = 1 and SR.IMASK = 0b1111
	stc	sr, r0		! get SR register
	or	r1, r0		! set mask for BL and IMASK
	ldc	r0, sr		! update SR regsiter

	! set process context into bankable register
	! because unbankable register will be over-written.
	mov	r8, r0

	! Load first process
	ldc.l	@r0+, R0_BANK	! set "process" r0 regsiter
	ldc.l	@r0+, R1_BANK	! set "process" r1 regsiter
	ldc.l	@r0+, R2_BANK	! set "process" r2 regsiter
	ldc.l	@r0+, R3_BANK	! set "process" r3 regsiter
	ldc.l	@r0+, R4_BANK	! set "process" r4 regsiter
	ldc.l	@r0+, R5_BANK	! set "process" r5 regsiter
	ldc.l	@r0+, R6_BANK	! set "process" r6 regsiter
	ldc.l	@r0+, R7_BANK	! set "process" r7 regsiter
	mov.l	@r0+, r8	! set r8 regsiter
	mov.l	@r0+, r9	! set r9 regsiter
	mov.l	@r0+, r10	! set r10 regsiter
	mov.l	@r0+, r11	! set r11 regsiter
	mov.l	@r0+, r12	! set r12 regsiter
	mov.l	@r0+, r13	! set r13 regsiter
	mov.l	@r0+, r14	! set r14 regsiter
	mov.l	@r0+, r15	! set r15 register
	ldc.l	@r0+, gbr	! set gbr regsiter
	lds.l	@r0+, macl	! set macl regsiter
	lds.l	@r0+, mach	! set mach regsiter
	ldc.l	@r0+, ssr	! set ssr regsiter
	ldc.l	@r0+, spc	! set spc regsiter
	lds.l	@r0+, pr	! set pr regsiter
	
	! Process switch
	rte
	nop

.align 4
.sr_msk:	.long 0x700000f0
.end
