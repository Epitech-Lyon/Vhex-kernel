#ifndef __OPCODE_H__
# define __OPCODE_H__

#include <stddef.h>
#include <stdint.h>

// Define the number max of arguments
// for intrustions.
#define ARGUMENTS_MAX	3

/* opcode - instruction data part. */
struct opcode_s
{
	const char *name;
	uint16_t mask;
	uint16_t code;
	uint16_t arg_mask[ARGUMENTS_MAX];
	void (*special)(char *buffer, uint16_t *area);
};

// Opcode list.
extern const struct opcode_s opcode_list[];

#endif /*__OPCODE_H__*/
