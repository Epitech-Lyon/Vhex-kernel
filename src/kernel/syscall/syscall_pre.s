.text
.global	_syscall_pre
.type	_syscall_pre, @function

.extern	_sys_get_handler
.extern	_process_current

.align 2
_syscall_pre:
	! save some used register.
	mov.l	r8, @-r15		! save r8 register
	mov.l	r9, @-r15		! save r9 register
	mov.l	r10, @-r15		! save r10 register
	sts.l	pr, @-r15		! save PR register

	! Call syscall high-level abstraction
	! to get appropriate system handler
	mov.l	.tra, r4		! r4 = TRA regsiter address (trapa exception code) 
	mov.l	.sys_get_handler, r0	! r0 = high-level abstration handler
	mov.l	@r4, r4			! r4 = trapa exception code
	jsr	@r0			! call abstraction
	shlr2	r4			! (db) r4 = syscall number
	cmp/eq	#0, r0			! if r0 == NULL...
	bt.s	syscall_pre_exit	! ...if yes, jump at <syscall_pre_exit>
	mov	r0, r9			! save kernel handler into unbankable register

	! We should restore the user stack
	mov.l	.process_current, r10	! get current_process adress
	mov.l	@r10, r10		! get current process
	mov.l	r15, @r10		! save current kernel stack
	mov.l	@(4, r10), r15		! restore user stack

	! Get and save SR register
	stc	sr, r0			! get SR register
	mov	r0, r8			! save SR register

	! Switch register bank and allow interrupt
	! @note: user context has been saved
	! during the `exception_handler_pre`
	! TODO: do not allow interrupt ?
	mov.l	.sr_mask, r1		! get SR mask for BL and IMASK
	and	r1, r0			! set SR.BL = 0, SR.RB = 0 and SR.IMASK = 0b0000
	ldc	r0, sr			! update SR regsiter

	! Call kernel abstraction
	jsr	@r9			! call system handler
	nop				! (db) nop.

	! Restore SR regsiter
	ldc	r8, sr			! SR.BL = 1, SR.RB = 1 and SR.IMASK = old mask.

	! Switch stack
	mov.l	r15, @(4, r10)		! save user stack
	mov.l	@r10, r15		! restore kernel stack


syscall_pre_exit:
	! Restore used regsiter.
	lds.l	@r15+, pr		! restore PR register
	mov.l	@r15+, r10		! restore r10 register
	mov.l	@r15+, r9		! restore r9 register
	mov.l	@r15+, r8		! restore r8 register

	! Exit properly
	rts				! exit
	nop				! (db) nop

.align 4
.tra:			.long 0xff000020
.sr_mask:		.long ~(0x300000f0)
.sys_get_handler:	.long _sys_get_handler
.process_current:	.long _process_current
.end
