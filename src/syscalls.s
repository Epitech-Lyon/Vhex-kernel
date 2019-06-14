.global	_casio_GetKey
.global	_casio_PrintMini
.global	_casio_Bdisp_PutDisp_DD
.global	_casio_Bdisp_AllClr_VRAM
.global	_casio_Bdisp_DrawLine_VRAM
.global	_casio_Bdisp_AreaClr_VRAM
.global	_casio_RestoreDisp
.global	_casio_SaveDisp
.global	_casio_Malloc
.global	_casio_Free

.type	_casio_GetKey, @function
.type	_casio_PrintMini, @function
.type	_casio_Bdisp_PutDisp_DD, @function
.type	_casio_Bdisp_AllClr_VRAM, @function
.type	_casio_Bdisp_DrawLine_VRAM, @function
.type	_casio_Bdisp_AreaClr_VRAM, @function
.type	_casio_RestoreDisp, @function
.type	_casio_SaveDisp, @function
.type	_casio_Malloc, @function
.type	_casio_Free, @function

.align 2
_casio_GetKey:
	mov.l	.syscall_tab, r1
	mov.l	.sys_getkey, r0
	jmp @r1
	nop
	
_casio_PrintMini:
	mov.l	.syscall_tab, r1
	mov.l	.sys_printMini, r0
	jmp @r1
	nop

_casio_Bdisp_AllClr_VRAM:
	mov.l	.syscall_tab, r1
	mov.l	.sys_clear, r0
	jmp @r1
	nop

_casio_Bdisp_PutDisp_DD:
	mov.l	.syscall_tab, r1
	mov.l	.sys_display, r0
	jmp @r1
	nop

_casio_Bdisp_DrawLine_VRAM:
	mov.l	.syscall_tab, r1
	mov.l	.sys_line, r0
	jmp @r1
	nop

_casio_Bdisp_AreaClr_VRAM:
	mov.l	.syscall_tab, r1
	mov.l	.sys_clear_area, r0
	jmp @r1
	nop

_casio_RestoreDisp:
	mov.l	.syscall_tab, r1
	mov.l	.sys_restore_disp, r0
	jmp @r1
	nop

_casio_SaveDisp:
	mov.l	.syscall_tab, r1
	mov.l	.sys_save_disp, r0
	jmp @r1
	nop

_casio_Malloc:
	mov.l	.syscall_tab, r1
	mov.l	.sys_malloc, r0
	jmp @r1
	nop

_casio_Free:
	mov.l	.syscall_tab, r1
	mov.l	.sys_free, r0
	jmp @r1
	nop


.align 4
.syscall_tab:		.long 0x80010070
.sys_line:		.long 0x00000030
.sys_clear:		.long 0x00000143 /*Bdisp_AllClr_VRAM */
.sys_getkey:		.long 0x0000090f
.sys_display:		.long 0x00000028
.sys_printMini:		.long 0x00000c4f
.sys_clear_area:	.long 0x0000014b
.sys_restore_disp:	.long 0x00000814
.sys_save_disp:		.long 0x00000813
.sys_malloc:		.long 0x00000acd
.sys_free:		.long 0x00000acc

.end
