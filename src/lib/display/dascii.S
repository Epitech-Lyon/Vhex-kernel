.text
.global	_dascii
.type	_dascii, @function

#include "kernel/unistd_32.h"

.align 2
_dascii:
	trapa	#__NR_kvram_ascii
	rts
	nop
.end
