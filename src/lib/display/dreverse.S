.text
.global	_dreverse
.type	_dreverse, @function

#include "kernel/unistd_32.h"

.align 2
_dreverse:
	trapa	#__NR_kvram_reverse
	rts
	nop
.end
