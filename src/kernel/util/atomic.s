.global	_atomic_start
.global	_atomic_end

.type	_atomic_start, @function
.type	_atomic_end, @function

.align 2
_atomic_start:
	stc	sr, r1		! Get SR register.
	mov.l	.bl_mask, r0	! Get SR.BL mask.
	or	r0, r1		! Set SR.BL to 1.
	ldc	r1, sr		! Update SR register.
	rts			! Exit.
	nop			! (db) nop.

_atomic_end:
	stc	sr, r1		! Get SR regiter.
	mov.l	.bl_mask, r0	! Get SR.BL register.
	not	r0, r0		! Get correct mask to remove the SR.BL bit.
	and	r0, r1		! Set SR.BL to 0.
	ldc	r1, sr		! Update SR register.
	rts			! Exit.
	nop			! (db) nop.

.align 4
.bl_mask:	.long (1 << 28)
.end
