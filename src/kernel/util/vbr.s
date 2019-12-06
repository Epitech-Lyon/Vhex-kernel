.global	_vbr_get
.global	_vbr_set

.type	_vbr_set, @function
.type	_vbr_get, @function

.text
.align 2
_vbr_get:
	stc	vbr, r0		! Get VBR register.
	rts			! Exit
	nop			! (db) nop

_vbr_set:
	! Block interrupt	
	stc	sr, r1		! Get SR register.
	mov	r1, r3		! Save current SR data.
	mov.l	.bl_mask, r2	! Get SR.BL mask.
	or	r2, r1		! Set SR.BL to 1.
	ldc	r1, sr		! Update SR register.

	! Get / Changle VBR
	stc	vbr, r0		! Get VBR register.
	ldc	r4, vbr		! Update VBR register.

	! Unblock interrupt.
	ldc	r3, sr		! Restore SR register.

	! Exit
	rts			! Exit
	nop			! (db) nop

.align 4
.bl_mask:	.long 0x100000f0
.end
