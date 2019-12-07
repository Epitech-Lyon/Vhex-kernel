#include <kernel/devices/ubc.h>
#include <kernel/hardware/ubc.h>
#include <kernel/keybios.h>
#include <kernel/syscall.h>
#include <kernel/extra.h>
#include <lib/display.h>


// Internal function.
extern void menu_context(ubc_session_t *session);
extern void menu_disassembly(ubc_session_t *session);

void ubc_handler(struct ubc_context_s *context, int channel)
{
	void (*menu)(ubc_session_t *session);
	ubc_session_t session;

	// Initialize new session.
	session.key = 0;
	session.channel = channel;
	session.context = context;
	session.menu.context.cursor = 0;
	session.menu.disassembly.cursor = 0;

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
	}
}
