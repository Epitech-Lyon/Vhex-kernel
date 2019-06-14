#ifndef __UTILS_H__
# define __UTILS_H__

#include <types.h>
#include <history.h>

#define SCREEN_WIDTH	128
#define SCREEN_HEIGHT	64
#define FONT_WIDTH	4
#define FONT_HEIGHT	6
#define LINE_OFFSET	((SCREEN_HEIGHT / FONT_HEIGHT) - 1)
#define COLUMN_OFFSET	(SCREEN_WIDTH / FONT_WIDTH)

#define KEY_UP		30018
#define KEY_DOWN	30023
#define KEY_LEFT	30020
#define KEY_RIGHT	30020
#define KEY_OPTN	30008

#define KEY_F1	30009
#define KEY_F2	30010
#define KEY_F3	30011
#define KEY_F4	30012
#define KEY_F5	30013
#define KEY_F6	30014

#define KEY_SHIFT	30006
#define KEY_ALPHA	30007
#define KEY_ENTER	30004
#define KEY_DEL		30025
#define KEY_SPACE	0x20
#define KEY_INS		30033
#define KEY_PI		0xd0
#define KEY_QUOTE	KEY_PI
#define KEY_EXIT	30002

#define KEY_A	 0x41
#define KEY_B	 0x42
#define KEY_C	 0x43
#define KEY_D	 0x44
#define KEY_E	 0x45
#define KEY_F	 0x46
#define KEY_G	 0x47
#define KEY_H	 0x48
#define KEY_I	 0x49
#define KEY_J	 0x4a
#define KEY_K	 0x4b
#define KEY_L	 0x4c
#define KEY_M	 0x4d
#define KEY_N	 0x4e
#define KEY_O	 0x4f
#define KEY_P	 0x50
#define KEY_Q	 0x51
#define KEY_R	 0x52
#define KEY_S	 0x53
#define KEY_T	 0x54
#define KEY_U	 0x55
#define KEY_V	 0x56
#define KEY_W	 0x57
#define KEY_X	 0x58
#define KEY_Y	 0x59
#define KEY_Z	 0x5a

#define KEY_0	 0x30
#define KEY_1	 0x31
#define KEY_2	 0x32
#define KEY_3	 0x33
#define KEY_4	 0x34
#define KEY_5	 0x35
#define KEY_6	 0x36
#define KEY_7	 0x37
#define KEY_8	 0x38
#define KEY_9	 0x39

#define ARGUMENTS_MAX		3
#define INSTRUCTION_SIZE	2
#define SESSIONS_SLOT		6
#define CMD_LENGHT_MAX		15

enum session_mode_e
{
	UNUSED,
	COMMAND,
	NORMAL,
};

struct session_s
{
	enum session_mode_e mode;
	ptrdiff_t anchor;
	int cursor;
};

enum insert_mode
{
	LETTER,
	CAPS_LOCK,
	NUMBER,
	SHIFT
};


struct vhex_s
{
	int current_session;
	struct session_s session[SESSIONS_SLOT];
	struct {
		char backup[CMD_LENGHT_MAX];
		char cmd[CMD_LENGHT_MAX];
		enum insert_mode mode;
	} insert;
	char info[CMD_LENGHT_MAX];
	struct history_s history;
};

// screen part.
void dclear(void);
void dupdate(void);

void display_instructions(const struct session_s *session);
void display_metainfos(const struct vhex_s *vhex, const struct session_s *session);
void key_handling(struct vhex_s *vhex, struct session_s *session, unsigned int key);
#endif /*__UTILS_H__*/
