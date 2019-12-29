.text
.global	_fork
.type	_fork, @function

#include "kernel/unistd_32.h"

.align 2
_fork:
	trapa	#__NR_fork
	rts
	nop
.end
