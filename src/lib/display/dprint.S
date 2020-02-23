#include "kernel/util/unistd_32.h"
.text
.global	_dprint
.type	_dprint, @function


.align 2
_dprint:
	trapa	#__NR_kvram_print
	rts
	nop
.end
