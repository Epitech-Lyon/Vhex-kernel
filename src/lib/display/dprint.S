.text
.global	_dprint
.type	_dprint, @function

#include "kernel/unistd_32.h"

.align 2
_dprint:
	trapa	#__NR_kvram_print
	rts
	nop
.end
