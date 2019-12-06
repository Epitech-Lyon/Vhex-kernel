.global	_icbi

.type	_icbi, @function

.align 2
_icbi:
	.word 0b0000010011100011	! SH4 intstruction "icbi @r4"
	rts				! exit
	nop				! (db) nop.
