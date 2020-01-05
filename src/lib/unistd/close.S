.text
.global	_close
.type	_close, @function

#include "kernel/unistd_32.h"

.align 2
_close:
	trapa	#__NR_close
	rts
	nop
.end
