.global	_test
.type	_test, @function

.align 2
_test:
	sts.l	pr, @-r15

	mov.l	.syscall_vram, r0
	mov.l	.syscall_entry, r1
	jsr	@r1
	nop
	tst	r0, r0
	bf	_test
	nop

	lds.l	@r15+, pr
	rts
	nop

.align 4
.syscall_entry:	.long 0xa0010070
.syscall_vram:	.long 0x00000135
.end
