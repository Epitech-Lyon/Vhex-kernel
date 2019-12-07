#include <kernel/devices/ubc.h>
#include <kernel/hardware/ubc.h>
#include <kernel/keybios.h>
#include <lib/display.h>

// Menu string format.
const char *content_string[] = {
	"!!!! UBC INTERRUPT !!!!",
	"Special registers:",
	"SPC           = %p",
	"Stack         = %p",
	"SSR           = %#x",
	"\n",
	"Generale registers:",
	"r0            = %#x",
	"r1            = %#x",
	"r2            = %#x",
	"r3            = %#x",
	"r4            = %#x",
	"r5            = %#x",
	"r6            = %#x",
	"r7            = %#x",
	"r8            = %#x",
	"r9            = %#x",
	"r10           = %#x",
	"r11           = %#x",
	"r12           = %#x",
	"r13           = %#x",
	"r14           = %#x",
	"r15           = %#x",
	"\n",
	"Debug informations:",
	"context       = %p",
	"UBC interrupt = %#x",
	"Channel       = %#x",
	NULL
};

//
// cursor_update - Update cursor position key pressed based.
// @note:
// 	Some limits are hard coded so be careful.
//
static void cursor_update(ubc_session_t *session)
{
	if (session->key == KEY_CTRL_UP &&
			session->menu.context.cursor > 0)
	{
		session->menu.context.cursor = session->menu.context.cursor - 1;
		return;
	}
	if (session->key == KEY_CTRL_DOWN &&
			session->menu.context.cursor < 28 - DISPLAY_VLINES_MAX)
	{
		session->menu.context.cursor = session->menu.context.cursor + 1;
		return;
	}
}

//
// menu_context - Show programe context.
// @note:
// This is not the best way to do the job,
// but for now the goal is the create a fonctionnal
// on-calc debugger.
//
void menu_context(ubc_session_t *session)
{
	// Menu string data
	uint32_t content_data[] = {
		0,
		0,
		session->context->spc,
		session->context->reg[15],
		session->context->ssr,
		0,
		0,
		session->context->reg[0],
		session->context->reg[1],
		session->context->reg[2],
		session->context->reg[3],
		session->context->reg[4],
		session->context->reg[5],
		session->context->reg[6],
		session->context->reg[7],
		session->context->reg[8],
		session->context->reg[9],
		session->context->reg[10],
		session->context->reg[11],
		session->context->reg[12],
		session->context->reg[13],
		session->context->reg[14],
		session->context->reg[15],
		0,
		0,
		(uint32_t)session->context,
		SH7305_UBC.CCMFR.LONG_WORD,
		session->channel
	};

	// Update cursor position.
	cursor_update(session);

	// Clear VRAM.
	dclear();

	// Display menu based on cursor position.
	for (int i = 0 ; content_string[i + session->menu.context.cursor] != NULL &&
			i < DISPLAY_VLINES_MAX ; i = i + 1)
	{
		dprint(0, i,
			content_string[i + session->menu.context.cursor],
			content_data[i + session->menu.context.cursor]
		);
	}

	// Display VRAM
	dupdate();
}
