#include <kernel/devices/ubc.h>
#include <kernel/devices/display.h>
#include <kernel/opcode.h>
#include <kernel/keybios.h>
#include <lib/display.h>
#include <lib/stdio.h>

static uint32_t get_arg(uint16_t code, const struct opcode_s *opcode, int id)
{
	int shift;

	// Check arg mask
	if (opcode->arg_mask[id] == 0x0000)
		return (0);

	// Get arg shift.
	shift = -1;
	while (++shift < 16 && !(opcode->arg_mask[id] & (0x01 << shift)));

	// Get argument.
	return ((code & opcode->arg_mask[id]) >> shift);
}

static void display_mnemonic(ubc_session_t *session)
{
	char line[128];
	uint16_t *area;
	int i;
	int j;

	// Get starting area.
	// TODO: update me !!
	area = (void *)(session->context->spc + (session->menu.disassembly.vcursor << 1));

	// Main Loop.
	i = -1;
	while (++i < DISPLAY_VCHAR_MAX)
	{
		// Generate first part.
		sprintf(line, "%8x %4x ", &area[i], area[i]);

		// Try to find opcode.
		j = -1;
		while (opcode_list[++j].name != NULL)
		{
			// Check opcode.
			if ((area[i] & opcode_list[j].mask) != opcode_list[j].code)
				continue;

			// Generate line via special function.
			if (opcode_list[j].special != NULL)
			{
				opcode_list[j].special(&line[14], &area[i]);
				break;
			}

			// Generate common line.
			sprintf(&line[14],
				opcode_list[j].name,
				get_arg(area[i], &opcode_list[j], 0),
				get_arg(area[i], &opcode_list[j], 1),
				get_arg(area[i], &opcode_list[j], 2)
			);
			break;
		}
		
		// If no opcode are found, generate "empty" line.
		if (opcode_list[j].name == NULL)
			sprintf(&line[14], ".word 0x%4x", area[i]);

		// Display line !
		dprint(session->menu.disassembly.hcursor, i, line);

		// Highlight break line if needed.
		if ((uint32_t)&area[i] == session->context->spc)
		{
			dreverse(
				0,
				i * (KERNEL_FONT_REAL_HEIGHT + 1),
				DISPLAY_SCREEN_WIDTH,
				KERNEL_FONT_REAL_HEIGHT + 1
			);
		}
	}
}

static void cursor_update(ubc_session_t *session)
{
	// Horizontal update.
	if (session->key == KEY_CTRL_LEFT)
		session->menu.disassembly.hcursor += 1;
	if (session->key == KEY_CTRL_RIGHT)
		session->menu.disassembly.hcursor -= 1;

	// Vertical update.
	if (session->key == KEY_CTRL_UP)
		session->menu.disassembly.vcursor -= 1;
	if (session->key == KEY_CTRL_DOWN)
		session->menu.disassembly.vcursor += 1;
}

void menu_disassembly(ubc_session_t *session)
{
	// Update cursor position.
	cursor_update(session);

	// display ASM.
	dclear();
	display_mnemonic(session);
	dupdate();
}
