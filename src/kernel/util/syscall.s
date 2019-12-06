.global	_casio_Bdisp_PrintMini
.global	_casio_Bdisp_PutDisp_DD
.global	_casio_Bdisp_AllClr_VRAM
.global	_casio_Bdisp_DrawLine_VRAM
.global	_casio_Bdisp_AreaReverseVRAM
.global	_casio_Bdisp_AreaClr_VRAM
.global	_casio_Bdisp_GetVRAM
.global	_casio_Bdisp_RestoreDisp
.global	_casio_Bdisp_SaveDisp

.global	_casio_Malloc
.global	_casio_Free

.global	_casio_GetKeyWait
.global	_casio_GetKey

.global	_casio_CreateFile
.global	_casio_OpenFile
.global	_casio_ReadFile
.global	_casio_WriteFile
.global	_casio_CloseFile




.type	_casio_PrintMini, @function
.type	_casio_Bdisp_PutDisp_DD, @function
.type	_casio_Bdisp_AllClr_VRAM, @function
.type	_casio_Bdisp_DrawLine_VRAM, @function
.type	_casio_Bdisp_AreaReverseVRAM, @function
.type	_casio_Bdisp_AreaClr_VRAM, @function
.type	_casio_Bdisp_GetVRAM, @function
.type	_casio_RestoreDisp, @function
.type	_casio_SaveDisp, @function

.type	_casio_Malloc, @function
.type	_casio_Free, @function

.type	_casio_GetKeyWait, @function
.type	_casio_GetKey, @function

.type	_casio_CreateFile, @function
.type	_casio_OpenFile, @function
.type	_casio_ReadFile, @function
.type	_casio_WriteFile, @function
.type	_casio_CloseFile, @function





.align 2
_casio_Bdisp_GetVRAM:
	mov.l	.syscall_entry, r1
	mov.l	.sys_getVRAM, r0
	jmp @r1
	nop


_casio_Bdisp_PrintMini:
	mov.l	.syscall_entry, r1
	mov.l	.sys_printMini, r0
	jmp @r1
	nop

_casio_Bdisp_AllClr_VRAM:
	mov.l	.syscall_entry, r1
	mov.l	.sys_clear, r0
	jmp @r1
	nop

_casio_Bdisp_PutDisp_DD:
	mov.l	.syscall_entry, r1
	mov.l	.sys_display, r0
	jmp @r1
	nop

_casio_Bdisp_DrawLine_VRAM:
	mov.l	.syscall_entry, r1
	mov.l	.sys_line, r0
	jmp @r1
	nop

_casio_Bdisp_AreaClr_VRAM:
	mov.l	.syscall_entry, r1
	mov.l	.sys_clear_area, r0
	jmp @r1
	nop

_casio_Bdisp_AreaReverseVRAM:
	mov.l	.syscall_entry, r1
	mov.l	.sys_reverse_area, r0
	jmp @r1
	nop

_casio_Bdisp_RestoreDisp:
	mov.l	.syscall_entry, r1
	mov.l	.sys_restore_disp, r0
	jmp @r1
	nop

_casio_Bdisp_SaveDisp:
	mov.l	.syscall_entry, r1
	mov.l	.sys_save_disp, r0
	jmp @r1
	nop


_casio_Malloc:
	mov.l	.syscall_entry, r1
	mov.l	.sys_malloc, r0
	jmp @r1
	nop

_casio_Free:
	mov.l	.syscall_entry, r1
	mov.l	.sys_free, r0
	jmp @r1
	nop


_casio_GetKey:
	mov.l	.syscall_entry, r1
	mov.l	.sys_getkey, r0
	jmp @r1
	nop
	
_casio_GetKeyWait:
	mov.l	.syscall_entry, r1
	mov.l	.sys_getkeywait, r0
	jmp @r1
	nop


_casio_CreateFile:
	mov.l	.syscall_entry, r1
	mov.l	.sys_createFile, r0
	jmp @r1
	nop

_casio_OpenFile:
	mov.l	.syscall_entry, r1
	mov.l	.sys_openFile, r0
	jmp @r1
	nop

_casio_WriteFile:
	mov.l	.syscall_entry, r1
	mov.l	.sys_writeFile, r0
	jmp @r1
	nop

_casio_ReadFile:
	mov.l	.syscall_entry, r1
	mov.l	.sys_readFile, r0
	jmp @r1
	nop

_casio_CloseFile:
	mov.l	.syscall_entry, r1
	mov.l	.sys_closeFile, r0
	jmp @r1
	nop





.align 4
.syscall_entry:		.long 0x80010070
.sys_getVRAM:		.long 0x00000135
.sys_line:		.long 0x00000030
.sys_clear:		.long 0x00000143 /*Bdisp_AllClr_VRAM */
.sys_getkey:		.long 0x0000090f
.sys_display:		.long 0x00000028
.sys_printMini:		.long 0x00000c4f
.sys_clear_area:	.long 0x0000014b
.sys_reverse_area:	.long 0x0000014d
.sys_restore_disp:	.long 0x00000814
.sys_save_disp:		.long 0x00000813
.sys_malloc:		.long 0x00000acd
.sys_free:		.long 0x00000acc
.sys_createFile:	.long 0x00000434
.sys_openFile:		.long 0x0000042c
.sys_writeFile:		.long 0x00000435
.sys_readFile:		.long 0x00000432
.sys_closeFile:		.long 0x0000042d
.sys_getkeywait:	.long 0x00000247
.end
