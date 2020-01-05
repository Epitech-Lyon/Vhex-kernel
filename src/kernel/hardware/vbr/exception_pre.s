.section ".vhex.exception", "awx", @progbits

.global	_exception_handler_pre
.type	_exception_handler_pre, @function

.extern	_exception_handler
.extern	_current_process
.extern	_syscall_pre

.align 2
_exception_handler_pre:
	! Due to TRAPA syscall API we should
	! switch stack to avoid variodic argument
	! corruption.
	mov.l	.process_current, r2		! get current_process address
	mov.l	@r2, r2				! get current process adrress
	tst	r2, r2				! if current process == NULL...
	bt	kernel_stack_switch_end		! ...if yes, jump at <kernel_stack_switch_end>
	mov.l	r15, @(4, r2)			! save current stack (user)
	mov.l	@r2, r15			! set kernel stack

kernel_stack_switch_end:
	! Save critical regsiter.
	stc.l	spc, @-r15			! save SPC register.
	stc.l	ssr, @-r15			! save SSR register.
	sts.l	pr, @-r15			! save pr register.
	mov.l	r8, @-r15			! save r8 register.

	! Save current process address
	mov	r2, r8				! save current process address

	! Check syscall (trapa)
	mov.l	.expevt, r0			! r0 = EXPEVT address.
	mov.l	.trapa_code, r1			! r1 = trapa exception code
	mov.l	@r0, r0				! r0 = excecption code
	cmp/eq	r1, r0				! if exception code == TRAPA CODE...
	bt	trapa_entry			! ...if yes, jump at <trapa_entry>

	! Call high-level abstraction
	mov.l	.exception_handler, r0		! get high-level aception abstraction
	jsr	@r0				! call abstraction
	nop					! (db) nop
	bra	exception_handler_exit		! jump at <exception_handler_exit>
	nop					! (db) nop.

trapa_entry:
	! Check current process
	tst	r2, r2				! if current process == NULL...
	bt	exception_handler_exit		! ...if yes, jump at <exception_handler_exit>

	! Call syscall pre handler
	mov.l	.syscall_pre, r0		! get syscall pre handler address
	jsr	@r0				! call pre handler
	nop					! (db) nop

exception_handler_exit:
	! "Restore" current process address
	mov	r8, r2				! restore current process address

	! Restore critical regsiter
	mov.l	@r15+, r8			! resotre r8 register.
	lds.l	@r15+, pr			! restore PR register.
	ldc.l	@r15+, ssr			! restore SSR regsiter.
	ldc.l	@r15+, spc			! restore SPC regsiter.

	! Restore user stack
	tst	r2, r2				! if current process == NULL...
	bt	exit_end			! ...if yes, jump at <exit_end>
	mov.l	r15, @r2			! save kernel stack
	mov.l	@(4, r2), r15			! restore user stack

	! Exit properly
exit_end:
	rte					! exit
	nop					! (db) nop

.align 4
.expevt:		.long 0xff000024
.trapa_code:		.long 0x00000160
.exception_handler:	.long _exception_handler
.syscall_pre:		.long _syscall_pre
.process_current:	.long _process_current
.end
