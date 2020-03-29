#ifndef __USER_UTIL_H__
# define __USER_UTIL_H__

#include <stddef.h>
#include <stdint.h>

extern int check_builtin(int argc, char **argv);
extern int strtotab(int *argc, char ***argv, char const *str);
extern void strtotab_quit(int *argc, char ***argv);

#endif /*__USER_UTIL_H__*/
