.text
.global	_dscroll
.type	_dscroll, @function

#include "kernel/unistd_32.h"

.align 2
_dclear:
	trapa	#__NR_kvram_scroll
	rts
	nop
.end
