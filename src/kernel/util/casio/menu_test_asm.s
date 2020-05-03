/*.text
.global	_casio_return_menu
.type	_casio_return_menu, @function

;;
;; casio_return_menu(int mode) - return to the Casio menu
;; mode 0 - involve menu manually
;; mode 1 - use GetKeyWait and inf. loop
;;
_casio_return_menu:
	mov.l	r8, @-r15
	sts.l	pr, @-r15

	! save the mode
	mov	r4, r8

	! Uninstall all drivers
	mov.l	.bootstrap_drivers_uninstall, r0
	jsr	@r0
	nop

	! TODO
	! TODO Handle mode !!
	! TODO

	! Save critical register 
	sts.l	mach, @-r15				! get mach regsiter
	sts.l	macl, @-r15				! get macl regsiter
	stc.l	gbr, @-r15				! get gbr regsiter
	mov.l	r14, @-r15				! get r14 regsiter
	mov.l	r13, @-r15				! get r13 regsiter
	mov.l	r12, @-r15				! get r12 regsiter
	mov.l	r11, @-r15				! get r11 regsiter
	mov.l	r10, @-r15				! get r10 regsiter
	mov.l	r9, @-r15				! get r9 regsiter
	mov.l	r8, @-r15				! get r8 regsiter

	! Save current stack
	mov.l 	.test_stack, r0
	mov.l	r15, @r0


	! Call GetKeyWait(int *row, int *col, int mode,
	! 			int timeout, int menu, int *keycode)
	add	#-4, r15				! get space
	mov	r15,r7					! r7 = &keycode
	add	#-4, r15				! get space
	mov	r15,r5					! r5 = &col
	add	#-4, r15				! get space
	mov	r15,r4					! r4 = &row
	mov	#0,r2					! r2 = menu = 0 (press [MENU] return to the menu)
	mov.l	r7,@-r15				! place the keycode address into the seventh aguments position
	mov.l	r2,@-r15				! place the menu option into the sixth arguments position
	mov	#0,r6					! r6 = mode = 0 (KEYWAIT_HALTON_TIMEROFF)
	mov	#0,r7					! r7 = timeout = 0

GetKeyWait:
	mov.l	.casio_GetKeyWait, r0			! get GetKeyWait syscall
	jsr	@r0					! GetKeyWait(int*col, int*row, int waittype, int timeout, int menu,u16*key)
	nop

	
	!add	#20, r15				! restore stack
	mov.l 	.test_stack, r0
	mov.l	@r0, r15

	! Restore critical register 
	mov.l	@r15+, r8				! set r8 regsiter
	mov.l	@r15+, r9				! set r9 regsiter
	mov.l	@r15+, r10				! set r10 regsiter
	mov.l	@r15+, r11				! set r11 regsiter
	mov.l	@r15+, r12				! set r12 regsiter
	mov.l	@r15+, r13				! set r13 regsiter
	mov.l	@r15+, r14				! set r14 regsiter
	ldc.l	@r15+, gbr				! set gbr regsiter
	lds.l	@r15+, macl				! set macl regsiter
	lds.l	@r15+, mach				! set mach regsiter

	! re-install all drivers
	mov.l	.bootstrap_drivers_install, r0
	jsr	@r0
	nop

	! epilogue
	lds.l	@r15+, pr
	rts
	nop

.align 4
.bootstrap_drivers_install:	.long _bootstrap_drivers_install
.bootstrap_drivers_uninstall:	.long _bootstrap_drivers_uninstall
.casio_GetKeyWait:		.long _casio_GetKeyWait
.test_stack:			.long _test_stack

.data
.align 4
.global _test_stack
.type	_test_stack, @object
_test_stack: .long 0
.end*/
