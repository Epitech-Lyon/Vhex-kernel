# Global options, used in each Makefile in the project
# If you have to change a tool name, or to add an option
# for every part of the project, this is the good place.

COMPILER	:= sh3eb-elf-
CC		:= $(COMPILER)gcc
OBJCPY		:= $(COMPILER)objcopy
AR		:= $(COMPILER)ar
LD		:= $(COMPILER)ld
WRAPPER		:= g1a-wrapper

CFLAGS		:= -Werror -Wall -W -Wextra -std=c18 -m3 -mb -mrenesas \
			-ffreestanding -nostdlib -fstrict-volatile-bitfields \
			-Wno-unused-const-variable

BUILD-DIR	:= build/
SRC-DIR		:= src/

red		:= \033[1;31m
green		:= \033[1;32m
blue		:= \033[1;34m
white		:= \033[1;37m
nocolor		:= \033[1;0m

define n
# Force new line character

endef
