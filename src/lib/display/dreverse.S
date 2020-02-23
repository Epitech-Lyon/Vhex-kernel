#include <kernel/util/unistd_32.h>
.text
.global	_dreverse
.type	_dreverse, @function


.align 2
_dreverse:
	trapa	#__NR_kvram_reverse
	rts
	nop
.end
