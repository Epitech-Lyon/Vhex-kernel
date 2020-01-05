.text
.global	_write
.type	_write, @function

#include "kernel/unistd_32.h"

.align 2
_write:
	trapa	#__NR_write
	rts
	nop
.end
