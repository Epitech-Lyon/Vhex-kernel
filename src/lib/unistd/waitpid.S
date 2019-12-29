.text
.global	_waitpid
.type	_waitpid, @function

#include "kernel/unistd_32.h"

.align 2
_waitpid:
	trapa	#__NR_waitpid
	rts
	nop
.end
