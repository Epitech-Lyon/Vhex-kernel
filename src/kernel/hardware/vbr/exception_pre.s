.section ".vhex.exception", "awx", @progbits

.global	_exception_handler_pre
.type	_exception_handler_pre, @function

.extern	_exception_handler
.extern	_syscall_pre

.align 2
_exception_handler_pre:
	! Save critical regsiter.
	stc.l	spc, @-r15		! save SPC register.
	stc.l	ssr, @-r15		! save SSR register.
	sts.l	pr, @-r15		! save pr register.

	! Check syscall (trapa)
	mov.l	.expevt, r0		! r0 = EXPEVT address.
	mov.l	.trapa_code, r1		! r1 = trapa exception code
	mov.l	@r0, r0			! r0 = excecption code
	cmp/eq	r1, r0			! if exception code == TRAPA CODE...
	bt	_trapa_entry		! ...if yes, jump at <trapa_entry>

	! Call high-level abstraction
	mov.l	.exception_handler, r0	! get high-level aception abstraction
	jsr	@r0			! call abstraction
	nop				! (db) nop
	bra	_exception_handler_exit	! jump at <_exception_handler_exit>
	nop				! (db) nop.

_trapa_entry:
	! Call syscall pre handler
	mov.l	.syscall_pre, r0	! get syscall pre handler address
	jsr	@r0			! call pre handler
	nop				! (db) nop


_exception_handler_exit:
	! Restore critical regsiter
	lds.l	@r15+, pr		! restore PR register.
	ldc.l	@r15+, ssr		! restore SSR regsiter.
	ldc.l	@r15+, spc		! restore SPC regsiter.

	! Exit properly
	rte				! exit
	nop				! (db) nop

.align 4
.expevt:		.long 0xff000024
.trapa_code:		.long 0x00000160
.exception_handler:	.long _exception_handler
.syscall_pre:		.long _syscall_pre
.end
