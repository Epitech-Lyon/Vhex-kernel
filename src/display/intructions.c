#include <utils.h>
#include <syscalls.h>
#include <opcode.h>
#include <string.h>

//TODO:
//	- move me !
//	- attribute always inline ?
static INLINE int get_shift(uint16_t mask)
{
	int i;

	if (mask == 0x0000)
		return (0);
	i = -1;
	while (++i < 16 && !(mask & (0x01 << i)));
	return (i);
}

static void get_mnemonic(char *str, uint16_t bytes_code)
{
	int shift[ARGUMENTS_MAX];
	size_t i;
	size_t j;

	i = -1;
	while (opcode[++i].name != NULL
	&& (bytes_code & opcode[i].mask) != opcode[i].code);
	if (opcode[i].name == NULL){
		strcpy(str, "????");
		return;
	}
	j = -1;
	while (++j < ARGUMENTS_MAX)
		shift[j] = get_shift(opcode[i].arg_mask[j]);
	//TODO: update this.
	sprintf(str, opcode[i].name,
	(bytes_code & opcode[i].arg_mask[0]) >> shift[0],
	(bytes_code & opcode[i].arg_mask[1]) >> shift[1],
	(bytes_code & opcode[i].arg_mask[2]) >> shift[2]);
}

//TODO:
//	- resize buffer.
//	- handle multiple modes.
void display_instructions(const struct session_s *session)
{
	char mnemonic[128];
	uint16_t *area;
	char buf[64];
	size_t y;

	if (session->mode == COMMAND){
		restore_window(1);
		return;
	}
	if (session->mode == UNUSED || session->anchor == 0x00000000){
		print((SCREEN_WIDTH >> 1) - ((sizeof("Empty session") * FONT_WIDTH) >> 1),
		(SCREEN_HEIGHT >> 1) - (FONT_HEIGHT >> 1), "Empty session");
		return;
	}
	y = -1;
	area = (void*)(session->anchor + session->cursor);
	while (++y < LINE_OFFSET){
		get_mnemonic(mnemonic, area[y]);
		sprintf(buf, "%4x %4x %s", &(area[y]), area[y], mnemonic);
		print(0, y * FONT_HEIGHT, buf);
	}
}
