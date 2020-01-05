.text
.global	_read
.type	_read, @function

#include "kernel/unistd_32.h"

.align 2
_read:
	trapa	#__NR_read
	rts
	nop
.end
