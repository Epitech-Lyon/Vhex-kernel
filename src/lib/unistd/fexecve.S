.text
.global	_fexecve
.type	_fexecve, @function

#include "kernel/unistd_32.h"

.align 2
_fexecve:
	trapa	#__NR_fexecve
	rts
	nop
.end
