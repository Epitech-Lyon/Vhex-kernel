##---
##	Code part.
##---
.text
.global	_atomic_start
.global	_atomic_end

.type	_atomic_start, @function
.type	_atomic_end, @function

.extern	_sr_save
.extern	_atomic_counter

.align 2
/*TODO: Fix reentrace corruption !!!*/
/*
** @proto: uint32_t atomic_start(void)
** @return:
**	* Saved SR register (if has been saved).
**	* 0xffffffff otherwise.
*/
_atomic_start:
	! Check if the calculator is currently
	! into an atomic operation and update
	! atomic counter.
	mov.l	.atomic_counter, r1	! r1 = atomic counter address
	mov.l	@r1, r2			! r2 = atomic counter data
	tst	r2, r2			! if atomic counter == 0...
	add	#1, r2			! update atomic counter data
	mov.l	r2, @r1			! update atomic counter.
	bf.s	atomic_start_exit	! ...if not, jump <atomic_start_exit>
	mov	#-1, r0			! (db) return 0xffffffff

	! Block interrupt if needed.
	stc	sr, r1			! get SR register.
	mov	r1, r0			! save current SR register.
	mov.l	.bl_mask, r2		! get SR.BL mask.
	or	r2, r1			! set SR.BL = 1 and SR.IMASK = 0b1111.
	ldc	r1, sr			! update SR register.

	! Save "old" SR register.
	mov.l	.sr_save, r1		! r1 = sr_save address
	mov.l	r0, @r1			! save "old" SR register data.

atomic_start_exit:
	rts				! exit.
	nop				! (db) nop.

/*TODO: Fix reentrace corruption !!!*/
/*
** @proto: uint32_t atomic_end(void)
** @return:
**	* Saved SR register if has been restored.
**	* 0xffffffff otherwise.
*/
_atomic_end:

	! Check if the calculator is currently
	! into an atomic operation and update
	! atomic counter if pssible.
	mov.l	.atomic_counter, r1	! r1 = atomic counter address
	mov.l	@r1, r0			! r0 = atomic counter data
	tst	r0, r0			! if atomic counter == 0...
	bt	atomic_end_error	! ...if yes, jump at <atomic_end_error>
	cmp/eq	#1, r0			! if atomic counter == 1...
	add	#-1, r0			! update atomic counter data
	mov.l	r0, @r1			! update atomic counter
	bf	atomic_end_error	! ...if not, jump at <atomic_end_error>

	! Restore saved SR register data.
	mov.l	.sr_save, r1		! get sr_save address
	mov.l	@r1, r0			! r0 = "old" SR register data
	ldc	r0, sr			! restore SR register.
	bra	atomic_end_exit		! return the resotred SR register.
	nop				! (db) nop

atomic_end_error:
	mov	#-1, r0			! return 0xffffffff

atomic_end_exit:
	rts				! exit.
	nop				! (db) nop.

.align 4
.bl_mask:		.long 0x100000f0
.sr_save:		.long _sr_save
.atomic_counter:	.long _atomic_counter




##---
##	Global part.
##---
.data
.global	_sr_save
.global _atomic_counter

.type	_sr_save, @object
.type	_atomic_counter, @object

.align 4
_sr_save:		.long 0x00000000
_atomic_counter:	.long 0x00000000



.end
