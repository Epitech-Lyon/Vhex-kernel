.text

.global	_sched_handler
.type	_sched_handler, @function

.extern _sched_timer_id
.extern _sched_timer_address
.extern _sched_schedule

.align 2
/*
** @note
** We can use only r0 - r7 register because
** we SHOULD not modify the current process context.
*/
_sched_handler:


/*
** Stop scheduler timer and clear interrupt flags
*/
stop_clear_sched_timer:
	! Clear scheduler timer interrupt
	mov.l	.sched_timer_address, r0		! get precalculated scheduler timer address
	mov.l	.timer_unf_field, r1			! get TIMER.TCR.UNF byte field
	mov.l	@r0, r0					! get scheduler timer TCR register
	mov.w	@r0, r2					! get scheduler timer TCR register
	not	r1, r1					! get interrupt flags bit mask (to remove)
	and	r1, r2					! clear timer interrupt flags
	mov.w	r2, @r0					! update timer TCR

	! Stop scheduler timer
	mov.l	.sched_timer_tstr_bit, r0		! get precalculated scheduler timer ID
	mov.l	.timer_tstr_register, r1		! get TMU0.TSTR base address
	mov.l	@r0, r0					! get scheduler timer ID
	mov.b	@r1, r2					! get TMU0.TSTR data
	not	r0, r0					! get TSTR scheduler timer ID mask (to remove)
	and	r0, r2					! stop scheduler timer
	mov.b	r2, @r1					! update TMU0.TSTR

#ifdef __SCHED_DEBUG__
	sts.l	pr, @-r15				! save pr register
	mov	#0, r4					! save r4 register (current context)
	mov	#0, r5					! save r5 register (next context)
	mov.l	.sched_debug, r0			! get sched_debug() absraction
	jsr	@r0					! call abstraction
	nop						! (db) nop
	lds.l	@r15+, pr				! restore pr register

	bra	restart_sched_timer
	nop
#endif


/*
** Get current and next context
*/
get_contexts:
	! prologue
	mov.l	r9, @-r15				! save r9 register
	mov.l	r8, @-r15				! save r8 register
	sts.l	pr, @-r15				! save pr register

	! Create to context_common_t **ptr
	add	#-4, r15				! create first context pointer
	mov	r15, r8					! save first context pointer
	add	#-4, r15				! create second context pointer
	mov	r15, r9					! save second context pointer
	add	#-4, r15				! create context pointer
	mov.l	r15, @r8				! save context pointer
	add	#-4, r15				! create context pointer
	mov.l	r15, @r9				! save context pointer
	
	! call high level abstraction
	mov	r8, r4					! send current contexts pointer
	mov	r9, r5					! send next context pointer
	mov.l	.sched_schedule, r0			! get sched_scheduler() abstraction address
	jsr	@r0					! call sched_schedule() abstract
	nop						! (db) nop

	! Restore stack and register (epilogue)
	mov.l	@r8, r4					! save current context
	mov.l	@r9, r5					! save next context
	add	#16, r15				! restore stack
	lds.l	@r15+, pr				! restore pr register
	mov.l	@r15+, r8				! restore r8 register
	mov.l	@r15+, r9				! restore r9 register

	! Check context error
	tst	r0, r0					! check if returned value == 0...
	bt	debug					! if yes, jump at <debug>
	bra	restart_sched_timer			! jump at <restart_sched_timer>
	nop						! (db) nop


/*
** Debug part, used to debug the contexts
*/
debug:
#ifdef __SCHED_DEBUG__
	sts.l	pr, @-r15				! save pr register
	mov.l	r4, @-r15				! save r4 register (current context)
	mov.l	r5, @-r15				! save r5 register (next context)
	mov.l	.sched_debug, r0			! get sched_debug() absraction
	jsr	@r0					! call abstraction
	nop						! (db) nop
	mov.l	@r15+, r5				! restore r5 register
	mov.l	@r15+, r4 				! restore r4 register
	lds.l	@r15+, pr				! restore pr register
#endif



/*
** Context switch
*/
save_current_context:
	! Check current process
	! @note: current process can be NULL !
	tst	r4, r4					! if current task == NULL...
	bt	next_context_load			! ...if yes, jump at <context_switch> 
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

next_context_load:
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


/*
** Restart timer
*/
restart_sched_timer:
	mov.l	.sched_timer_tstr_bit, r0		! get precalculated scheduler timer ID
	mov.l	.timer_tstr_register, r1		! get TMU0.TSTR base address
	mov.l	@r0, r0					! get scheduler timer ID
	mov.b	@r1, r2					! get TMU0.TSTR data
	or	r0, r2					! start scheduler timer
	mov.b	r2, @r1					! update TMU0.TSTR


/*
** Process switch
*/
process_switch:
	rte						! use RTE to switch process
	nop						! (db) nop


.align 4
.sched_timer_tstr_bit:	.long _sched_timer_tstr_bit
.sched_timer_address:	.long _sched_timer_address
.sched_schedule:	.long _sched_schedule
.timer_tstr_register:	.long 0xa4490004
.timer_unf_field:	.long 0x00000100
#ifndef __SCHED_DEBUG__
.sched_debug:		.long _sched_debug
#endif
.end
