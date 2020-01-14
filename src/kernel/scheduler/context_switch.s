.text

.global	_sched_context_switch
.type	_sched_context_switch, @function

.align 2
/*
** void sched_context_switch(common_context_t *current, common_context_t *next);
**
** @note:
** r4 - current scheduler task
** r5 - next task
** r0 - tmp
*/
_sched_context_switch:
	! Save contexts arg
	mov.l	r8, @-r15				! save r8
	mov.l	r9, @-r15				! save r9
	mov	r4, r8					! save current context into unbankable reg
	mov	r5, r9					! save next context into unbankable reg


	! Update SR register to block interrupt / exception and
	! switch register bank to simalate system call.
	! @note:
	!	Normaly this is the TMU0 interrupt so the interrupt
	! and register banck is already switchted BUT durring the
	! bootstrap part, the kernel call this function in "user"
	! context. This is why we force update CPU configuration.
	mov.l	.sr_msk, r1				! get mask for SR.BL = 1, SR.RB = 1 and SR.IMASK = 0b1111
	stc	sr, r0					! get SR register
	or	r1, r0					! set mask for BL, RB and IMASK
	ldc	r0, sr					! update SR regsiter


	! Restore context arg
	mov	r8, r4					! restore current context	
	mov	r9, r5					! restore next context
	mov.l	@r15+, r9				! restore r9
	mov.l	@r15+, r8				! restore r8

save_current_context:
	! Check current process
	! @note: current process can be NULL !
	tst	r4, r4					! if current task == NULL...
	bt	context_load				! ...if yes, jump at <context_switch> 
	add	#88, r4					! get &process->context (end)

	! save current context
	sts.l	pr, @-r4				! get pr regsiter
	stc.l	spc, @-r4				! get spc regsiter
	stc.l	ssr, @-r4				! get ssr regsiter
	sts.l	mach, @-r4				! get mach regsiter
	sts.l	macl, @-r4				! get macl regsiter
	stc.l	gbr, @-r4				! get gbr regsiter
	mov.l	r15, @-r4				! get r15 register
	mov.l	r14, @-r4				! get r14 regsiter
	mov.l	r13, @-r4				! get r13 regsiter
	mov.l	r12, @-r4				! get r12 regsiter
	mov.l	r11, @-r4				! get r11 regsiter
	mov.l	r10, @-r4				! get r10 regsiter
	mov.l	r9, @-r4				! get r9 regsiter
	mov.l	r8, @-r4				! get r8 regsiter
	stc.l	R7_BANK, @-r4				! get "process" r7 regsiter
	stc.l	R6_BANK, @-r4				! get "process" r6 regsiter
	stc.l	R5_BANK, @-r4				! get "process" r5 regsiter
	stc.l	R4_BANK, @-r4				! get "process" r4 regsiter
	stc.l	R3_BANK, @-r4				! get "process" r3 regsiter
	stc.l	R2_BANK, @-r4				! get "process" r2 regsiter
	stc.l	R1_BANK, @-r4				! get "process" r1 regsiter
	stc.l	R0_BANK, @-r4				! get "process" r0 regsiter


context_load:
	! Load next process
	! @note: next process can not be NULL !
	ldc.l	@r5+, R0_BANK				! set "process" r0 regsiter
	ldc.l	@r5+, R1_BANK				! set "process" r1 regsiter
	ldc.l	@r5+, R2_BANK				! set "process" r2 regsiter
	ldc.l	@r5+, R3_BANK				! set "process" r3 regsiter
	ldc.l	@r5+, R4_BANK				! set "process" r4 regsiter
	ldc.l	@r5+, R5_BANK				! set "process" r5 regsiter
	ldc.l	@r5+, R6_BANK				! set "process" r6 regsiter
	ldc.l	@r5+, R7_BANK				! set "process" r7 regsiter
	mov.l	@r5+, r8				! set r8 regsiter
	mov.l	@r5+, r9				! set r9 regsiter
	mov.l	@r5+, r10				! set r10 regsiter
	mov.l	@r5+, r11				! set r11 regsiter
	mov.l	@r5+, r12				! set r12 regsiter
	mov.l	@r5+, r13				! set r13 regsiter
	mov.l	@r5+, r14				! set r14 regsiter
	mov.l	@r5+, r15				! set r15 register
	ldc.l	@r5+, gbr				! set gbr regsiter
	lds.l	@r5+, macl				! set macl regsiter
	lds.l	@r5+, mach				! set mach regsiter
	ldc.l	@r5+, ssr				! set ssr regsiter
	ldc.l	@r5+, spc				! set spc regsiter
	lds.l	@r5+, pr				! set pr regsiter


	! Process switch
	rte						! use RTE to switch process
	nop						! (db) nop

.align 4
.sr_msk:		.long 0x700000f0
.end
