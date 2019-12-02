.globl	_section_wipe
.global	_section_load
.type	_section_load, @function
.type	_section_wipe, @function

.align 2
/*
** section_map(uint32_t *dest, uint32_t *src, size_t size)
** Load section using his symbols.
*/
_section_load:
	shlr2	r6		! sections are force 4-aligned by the linker. 
	tst	r6, r6		! check null section...
	bt	load_loop_end	! ...if null, exit.

load_loop_entry:
	mov.l	@r5, r1		! get "src" data.
	mov.l	r1, @r4		! copy into the "dest" section.
	add	#4, r5		! update "src" address.
	dt	r6		! check the load size.
	bf/s	load_loop_entry	! loop jump.
	add	#4, r4		! update "dest" address.

load_loop_end:
	rts			! clean exit.
	nop			! delayed branch.
	

/*
** section_wipe(uint32_t *bsection, size_t size)
** Wipe section using his symbols.
*/
_section_wipe:
	mov	#0, r0		! wipe data.
	shlr2	r5		! the section force 4-aligned by the linker.
	tst	r5, r5		! Check null section...
	bt	wipe_loop_end	! ...if null, exit !

wipe_loop_entry:
	mov.l	r0, @r4		! wipe section part.
	dt	r5		! check the section size.
	bf/s	wipe_loop_entry	! loop jump.
	add	#4, r4		! address update.

wipe_loop_end:
	rts			! clean exit.
	nop			! delayed branch.
	
.end
