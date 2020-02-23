#include <kernel/util/unistd_32.h>
.text
.global	_dascii
.type	_dascii, @function


.align 2
_dascii:
	trapa	#__NR_kvram_ascii
	rts
	nop
.end
