.text
.global	_open
.type	_open, @function

#include "kernel/unistd_32.h"

.align 2
_open:
	trapa	#__NR_open
	rts
	nop
.end
