#include <kernel/devices/ubc.h>
#include <kernel/hardware/ubc.h>
#include <kernel/syscall.h>
#include <kernel/extra.h>
#include <lib/display.h>

void ubc_handler(ubc_context_t *context, int channel)
{
	unsigned int key;

	dclear();
	dprint(0, 0,
		"UBC INTERRUPT !!!!\n"
		"context.spc   = %p\n"
		"context.ssr   = %p\n"
		"context.stack = %p\n"
		"context       = %p\n"
		"UBC interrupt = %#x\n"
		"Channel       = %#x\n",
		context->spc,
		context->ssr,
		context->reg[15],
		context,
		SH7305_UBC.CCMFR.LONG_WORD,
		channel
	);
	dupdate();
	while (1)
	{
		casio_GetKey(&key);
	}
}
