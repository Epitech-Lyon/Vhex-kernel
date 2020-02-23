#include <kernel/devices/ubc.h>
#include <kernel/devices/display.h>
#include <kernel/hardware/ubc.h>
#include <kernel/syscall.h>
#include <kernel/util/extra.h>


// Internal function.
extern void menu_context(ubc_session_t *session);
extern void menu_disassembly(ubc_session_t *session);

void ubc_module_handler(int action)
{
/*	dclear();
	dprint(0, 0, "spc = %p", action);
	dupdate();
	for (int i = 0 ; i < 4000000 ; i = i + 1);

*/	//TODO stop all clock / timer.
}

void ubc_handler(struct ubc_context_s *context, int channel)
{
/*	void (*menu)(ubc_session_t *session);
	ubc_session_t session;

	// Initialize new session.
	session.key = 0;
	session.channel = channel;
	session.context = context;
	session.menu.context.cursor = 0;
	session.menu.disassembly.vcursor = -(DISPLAY_VCHAR_MAX >> 1);
	session.menu.disassembly.hcursor = 0;

	// Initialize menu function.
	menu = &menu_disassembly;

	// Main loop.
	while (session.key != KEY_CTRL_EXIT)
	{
		menu(&session);
		casio_GetKey(&session.key);
		switch (session.key)
		{
			case KEY_CTRL_F1: menu = &menu_disassembly; break;
			case KEY_CTRL_F2: menu = &menu_context; break;
		}
	}*/

	// Update UBC
	SH7305_UBC.CBR0.CE	= 0; 		// Disable channel.
//	SH7305_UBC.CAR0		= context->spc;	// Update break address.
//	SH7305_UBC.CAMR0	= 0x00000000;	// Update break address.
//	SH7305_UBC.CBR0.CE	= 1; 		// Enable channel.
	icbi((void*)0xa0000000);
}
