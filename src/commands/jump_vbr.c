#include <command.h>
#include <string.h>

// Internal prototype
static void vbrjmp(int argc, char **argv, struct session_s *session, char *info);

// TODO: write doc.
CMDBLOCK("vbrjmp", &vbrjmp,
"VBR JUMP command help\n"
"This command jump into Casio's\nhandlers.\n"
"\n"
"Due to the SH3 / SH4 VBR system\n, there are 3 vectors offset\n"
"called by the processor:\n"
"    * exception (VBR + 0x100)\n"
"    * tlb miss  (VBR + 0x400)\n"
"    * interrupt (VBR + 0x600)\n"
"\n"
"You can access to each handler\nusing this command list:\n"
":vbrjmp except  VBR + 0x100\n"
":vbrjmp fault   VBR + 0x400\n"
":vbrjmp int     VBR + 0x600\n"
);

//
// vbr_jump() - jump to the exception, tlb or interrupt Casio handler.
// Due to the SH3 / SH4 VBR system, there are 3 vectors offset called
// by the processor:
// 	* vbr + 0x100 -> general exeptions.
// 	* vbr + 0x400 -> tlb miss.
//	* vbr + 0x600 -> interrupts.
//
static void vbrjmp(int argc, char **argv, struct session_s *session, char *info)
{
	if (argc != 2){
		strcpy(info, "arguments error");
		return;
	}
	if (strcmp(argv[1], "fault") && strcmp(argv[1], "except") && strcmp(argv[1], "int")){
		sprintf(info, "bad argument");
		return;
	}
	__asm__ volatile ("stc vbr, %0" : "=r"(session->anchor));
	if (!strcmp(argv[1], "except"))
		session->anchor += 0x100;
	if (!strcmp(argv[1], "fault"))
		session->anchor += 0x400;
	if (!strcmp(argv[1], "int"))
		session->anchor += 0x600;
}
