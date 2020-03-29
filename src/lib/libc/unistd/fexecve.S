#include <asm/unistd_32.h>
.text
.global	_fexecve
.type	_fexecve, @function


.align 2
_fexecve:
	trapa	#__NR_fexecve
	rts
	nop
.end
