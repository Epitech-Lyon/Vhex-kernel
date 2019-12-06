.global	_dbr_set
.global	_dbr_get
.type	_dbr_set, @function
.type	_dbr_get, @function

.text
.align 2
_dbr_set:
	.word 0b0000000011111010	! SH4 intruction "stc dbr, r0"
	.word 0b0100010011111010	! SH4 intruction "ldc r4, dbr"
	rts
	nop

_dbr_get:
	.word 0b0000000011111010	! SH4 intruction "stc dbr, r0"
	rts
	nop

.end
