.text
.global	_dclear
.type	_dclear, @function

#include "kernel/unistd_32.h"

.align 2
_dclear:
	trapa	#__NR_kvram_clear
	rts
	nop
.end
