.text

.global _t6k11_lcd_driver
.type	_t6k11_lcd_driver, @function

.align 2
/*
** @proto: void _t6k11_lcd_driver(void *vram)
**
** @note:
**	This function will display the vram on the screen.
** This is the T6K11 drivers, *DO NOT USE* this function if
** you are not absolutly sure that what you are doing.
**
** @register list:
**	0  - Display mode
**	1  - Counter mode
** 	2  - Set analog control mode
**	3  - Set alternating signal mode
**	4  - Set X-address AND Set Y-address
**	5  - Set Z-address
**	6  - Contrast control
**	7  - Data Write AND Data Read
**	8  - Unknown, test mode only
**	9  - Unknown, test mode only
**	10 - Unknown, test mode only
**	11 - Unknown, test mode only
**	12 - D/A converter power control
**	13 - Unknown, test mode only.
**	14 - Unknown, test mode only.
**	15 - Unknown, test mode only.
**
*/
_t6k11_lcd_driver:
	! To avoid reentrace with the Video
	! RAM and I/O opetation, we need to
	! display data using atomic operation.
	! The next part *SHOULD* be exception save !
	mov.l	.sr_int_msk, r1			! Get SR mask for interrupt blocking.
	stc	sr, r0				! Save current SR register data.
	mov	r0, r2				! Get SR register data.
	or	r1, r2				! Set SR.BL = 1 and SR.IMASK = 0b1111
	ldc	r2, sr				! Update SR register

	! Stack management
	mov.l	r8, @-r15			! save r8 register.
	mov.l	r0, @-r15			! save SR register.

	! Get LCD register addresses.
	mov.l .LCD_register_selector, r0	! r0 = LCD register selector address.
	mov.l .LCD_data_register, r1		! r1 = LCD data register.

	! Set the X-adress counter to up-mode.
	! @note:
	!	The X-address counter is used
	! for the row and the Y-addresss counter
	! for the column.
	mov	#1, r2				! r0 = counter mode register address.
	mov.b	r2, @r0				! Select the counter register.
	mov	#1, r3				! r0 = Y-address counter "Yup" mode.
	mov.b	r3, @r1				! Set the Y-address counter register to "Yup" mode.

	
	! "pre-generate" register data
	! r0 - LCD register selector
	! r1 - LCD data register
	! r2 - X-address / Y-address counter register
	! r3 - data write address
	! r4 - vram
	! r5 - row counter
	! r6 - Y-address counter
	! r7 - X-address counter
	mov	#4, r2				! X-address / Y-address counter.
	mov	#7, r3				! Data Write register
	mov	#64, r5				! Row counter
	mov	#0, r6				! Y-address counter (column, alway 0)
	mov	#192, r7			! X-address counter (row, 192 = 0, see documentation)

row_loop:
	! Select the X-address / Y-address counter.
	mov.b	r2, @r0				! Select X/Y address counter register.

	! Update X-address counter / Reset the Y-address counter
	mov.b	r7, @r1				! Set X-address counter to current row
	mov.b	r6, @r1				! Set Y-address counter to 0

	! Select Data write register.
	mov.b	r3, @r0				! Select Data Write register

	! Avoid loop, so write line manually.
	mov.b	@r4+, r8
	mov.b	r8, @r1
	mov.b	@r4+, r8
	mov.b	r8, @r1
	mov.b	@r4+, r8
	mov.b	r8, @r1
	mov.b	@r4+, r8
	mov.b	r8, @r1

	mov.b	@r4+, r8
	mov.b	r8, @r1
	mov.b	@r4+, r8
	mov.b	r8, @r1
	mov.b	@r4+, r8
	mov.b	r8, @r1
	mov.b	@r4+, r8
	mov.b	r8, @r1

	mov.b	@r4+, r8
	mov.b	r8, @r1
	mov.b	@r4+, r8
	mov.b	r8, @r1
	mov.b	@r4+, r8
	mov.b	r8, @r1
	mov.b	@r4+, r8
	mov.b	r8, @r1

	mov.b	@r4+, r8
	mov.b	r8, @r1
	mov.b	@r4+, r8
	mov.b	r8, @r1
	mov.b	@r4+, r8
	mov.b	r8, @r1
	mov.b	@r4+, r8
	mov.b	r8, @r1

	! Check next line.
	dt	r5		! if (--r5) == 0...
	bf.s	row_loop	! ...if not, jump at <row_loop>
	add	#1, r7		! (db) update row counter.

exit_part:
	! Exit properly
	ldc.l	@r15+, sr	! restore SR register
	mov.l	@r15+, r8	! resotre r8 register
	rts			! exit.
	nop			! (db) nop.

.align 4
.LCD_register_selector:	.long 0xb4000000	/* LCD register selector */
.LCD_data_register:	.long 0xb4010000	/* LCD data register */
.sr_int_msk:		.long 0x100000f0	/* SR interrupt mask */
.end


/*
NOTE: OLD BUT FAST AND NOT SAFE
NOTE: FOR MULTIPLE PROCESS !
	_t6k11_display(void *vram)
	This function will display the vram on the screen.
	This is the T6K11 drivers, DO NOT USE this function if you are
	not absolutly sure that what you are doing.

	TODO: use DMA ?

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
*/
!.align 4
!1:	.long 0xb4000000	/* LCD register selector */
!2:	.long 0xb4010000	/* LCD data register */
