#include <command.h>
#include <string.h>

// Internal prototype.
static void address_jump(int argc, char **argv, struct session_s *session, char *info);

// Define the command block into the cache.
CMDBLOCK("jmp", &address_jump,
"JMP command help\n"
"This command takes one\nparameter: the address (hexa)\n"
"you would jump into the Virtual Memory.\n"
"\n"
"Be careful, there is no security"
"and some parts of the Virtual\n"
"Memory can make your\ncalculator crash.\n"
"\n"
"If a crash occurs, don't worry, you have \"just\" read a\n"
"non-readable space.\n"
"Your calculator will continue\nto work perfectly after\nthe reset.\n"
"\n"
"Virtual Memory map:\n"
"Area P0 - 2Go - MMU enable\n"
"0x00300000 : add-in header\n"
"0x00300200 : add-in mapping\n"
"0x08000000 : RAM (cache)\n"
"Area P1 - 500Mo - MMU disable\n"
"0x80000000 : ROM (cache)\n"
"0x88000000 : RAM (cache)\n"
"Area P2 - 500Mo - MMU disable\n"
"0xa0000000 : ROM (no cache)\n"
"0xa8000000 : RAM (no cache)\n"
"Area P3 - 500Mo - MMU ebable\n"
"?????? CRASH ??????\n"
"Area P4 - 500Mo - MMU ~enable\n"
"0xe0000000 : Store queue area\n"
"0xe4000000 : MMU disable\n"
"0xe5000000 : On chip RAM\n"
"0xe6000000 : MMU disable\n"
);

static INLINE int check_address(char *str)
{
	--str;
	while ((*(++str) >= '0' && *str <= '9') || (*str >= 'a' && *str <= 'f'));
	return ((*str != '\0') ? 1 : 0);
}

/* address_jump() - jump into Virtual Memory. */
static void address_jump(int argc, char **argv, struct session_s *session, char *info)
{
	if (argc != 2 || check_address(argv[1]) != 0){
		strcpy(info, "arguments error");
		return;
	}
	// clear the lower bits to force align the address.
	// (one instruction takes 2 bytes, so we need to respect the alignment).
	session->anchor = atoi_base(argv[1], 16) >> 1 << 1;
}
