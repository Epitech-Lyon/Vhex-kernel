.section ".vhex.interrupt", "awx", @progbits

.global	_interrupt_handler_pre
.type	_interrupt_handler_pre, @function

.extern	_sched_timer_intevt
.extern	_sched_handler
.extern	_interrupt_handler

_interrupt_handler_pre:
	! Due to PR register used to switch process
	! context, we SHOULD catch scheduler timer
	! interrupt before the interrupt handler because
	! we need to save / restore pr register before
	! context swtich.
	mov.l	.sched_timer_intevt, r0		! get the precalculate scheduler timer event
	mov.l	.intevt_register, r1		! get INTEVT register address
	mov.l	@r0, r0				! get the scheduler event code
	mov.l	@r1, r1				! get the interrupt event
	cmp/eq	r0, r1				! if scheduler timer event == INTEVT...
	bf	general_interrupt		! ...if not, jump at <general_interrupt>

scheduler_interrupt:
	mov.l	.scheduler_handler, r0		! get scheduler handler address
	jmp	@r0				! jump into scheduler handler
	nop					! (db) nop

general_interrupt:
	! Save critical regsiter.
	stc.l	spc, @-r15			! save SPC register.
	stc.l	ssr, @-r15			! save SSR register.
	sts.l	pr, @-r15			! save pr register.

	! Call interrupt handler
	mov.l	.interrupt_handler, r0		! get interrupt handler address
	jsr	@r0				! call interurpt handler
	nop					! (db) nop

	! Restore critical regsiter
	lds.l	@r15+, pr			! restore PR register.
	ldc.l	@r15+, ssr			! restore SSR regsiter.
	ldc.l	@r15+, spc			! restore SPC regsiter.

	! Return
	rte					! exit
	nop					! (db) nop

.align 4
.sched_timer_intevt:	.long _sched_timer_intevt
.scheduler_handler:	.long _sched_handler
.interrupt_handler:	.long _interrupt_handler
.intevt_register:	.long 0xff000028
