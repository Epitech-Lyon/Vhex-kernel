.global _t6k11_display
.type	_t6k11_display, @function

.text
.align 2
/*
	_t6k11_display(void *vram)
	This function will display the vram on the screen.
	This is the T6K11 drivers, DO NOT USE this function if you are
	not absolutly sure that what you are doing.

	TODO: use DMA ?
*/
# register organisation:
#	r0	Y axis loop.
#	r1	LCD register selector. 
#	r2	LCD data register.
#	r3	X axis loop.
#	r4	VRAM address.
_t6k11_display:
	mov.l r8, @-r15
	mov.l r9, @-r15
	mov.l 1f, r1
	mov.l 2f, r2
	mov #4, r6
	mov #0, r7
	mov #7, r8
	mov #192, r5
	mov #64, r0
	loop_y:

		mov.b r6, @r1
		mov.b r5, @r2
		mov.b r6, @r1
		mov.b r7, @r2
		mov.b r8, @r1
		mov #16, r3
		loop_x:

			mov.b @r4+, r9
			mov.b r9, @r2

		dt r3
		bf loop_x

	dt r0
	bf.s loop_y
	add #1, r5
	mov.l @r15+, r9
	rts
	mov.l @r15+, r8

.align 4
1:	.long 0xb4000000	/* LCD register selector */
2:	.long 0xb4010000	/* LCD data register */
