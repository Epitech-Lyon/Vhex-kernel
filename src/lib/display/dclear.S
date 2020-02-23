#include <kernel/util/unistd_32.h>
.text
.global	_dclear
.type	_dclear, @function


.align 2
_dclear:
	trapa	#__NR_kvram_clear
	rts
	nop
.end
