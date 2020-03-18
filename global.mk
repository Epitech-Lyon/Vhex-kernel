# Global option use in each Makefile

# Tools
COMPILER	:= sh3eb-elf-
CC		:= $(COMPILER)gcc
LD		:= $(COMPILER)ld
OBJCOPY		:= $(COMPILER)objcopy
OBJDUMP		:= $(COMPILER)objdump
WRAPPER		:= g1a-wrapper

# Flags
CFLAGS		:= -Werror -Wall -W -Wextra -std=c18 -m3 -mb -mrenesas \
			-ffreestanding -nostdlib -fstrict-volatile-bitfields \
			-Wno-unused-const-variable -Wno-unused-function \
			-Wno-unused-variable -Wno-unused-but-set-variable \
			-Wno-unused-parameter

# Colors
red		:= \033[1;31m
green		:= \033[1;32m
blue		:= \033[1;34m
white		:= \033[1;37m
nocolor		:= \033[1;0m


define n
# Force newline character

endef
