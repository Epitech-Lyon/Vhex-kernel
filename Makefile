#!/usr/bin/make -f
## ---
##	Project: Vhex - On-calc debugger
##	Author: yann.magnin@epitech.eu
## ---

##---
##	Static variables
##--
HEADER		:= include
BUILD		:= build
DEBUG		:= debug

NAME		:= vhex
EXEC		:= $(NAME).g1a
LDFLAG		:= -T $(NAME).ld
MEMORY_MAP	:= $(DEBUG)/$(NAME).map
ICON		:= icon.bmp


COMPILER	:= sh3eb-elf-
CC		:= $(COMPILER)gcc
OBJCOPY		:= $(COMPILER)objcopy
OBJDUMP		:= $(COMPILER)objdump
WRAPPER		:= g1a-wrapper
CFLAGS		:= -Werror -Wall -W -Wextra -std=c18 -m3 -mb -mrenesas \
			-ffreestanding -nostdlib -fstrict-volatile-bitfields \
			-Wno-unused-const-variable -Wno-unused-function \
			-Wno-unused-variable -Wno-unused-but-set-variable \
			-Wno-unused-parameter


red		:= \033[1;31m
green		:= \033[1;32m
blue		:= \033[1;34m
white		:= \033[1;37m
nocolor		:= \033[1;0m




##---
##	Automated variables
##---
SRC		:=
DIRECTORY	:= $(shell find src -not -path "*/\.*" -type d)
# Get all source files
$(foreach path,$(DIRECTORY),$(eval		\
	SRC	+= $(wildcard $(path)/*.c)	\
			$(wildcard $(path)/*.S)	\
			$(wildcard $(path)/*.s)	\
))
# Geneate all object files
OBJ	:= $(patsubst %,$(BUILD)/%.o,$(subst /,_,$(subst src/,,$(basename $(SRC)))))




##---
##	General rules
##---
all: | $(BUILD) $(DEBUG) $(EXEC)

$(EXEC): $(OBJ)
	$(CC) -Wl,-M $(LDFLAG) $(CFLAGS) -o $(DEBUG)/$(NAME).elf $(OBJ) -I $(HEADER) -lgcc > $(MEMORY_MAP)
	$(OBJCOPY) -R .comment -R .bss -O binary $(DEBUG)/$(NAME).elf $(DEBUG)/$(NAME).bin
	$(WRAPPER) $(DEBUG)/$(NAME).bin -o $@ -i $(ICON)

$(BUILD) $(DEBUG):
	@ printf "Create $(blue)$@$(nocolor) directory\n"
	@ mkdir -p $@

install: $(EXEC)
	sudo p7 send --force $^

check:
	@ echo 'src: $(SRC)'
	@ echo 'obj: $(OBJ)'
	@ echo 'directory: $(DIRECTORY)'

map:
	@ $(OBJDUMP) -D $(DEBUG)/$(NAME).elf | less

bin:
	@ cat $(MEMORY_MAP) | less


##---
##	  Automated rules
##---
define rule-src
$(patsubst %,$(BUILD)/%.o,$(subst /,_,$(subst src/,,$(basename $1)))): $1
	@ printf "compiling $(white)$$<$(nocolor)..."
	@ $(CC) $(CFLAGS) -o $$@ -c $$< -I $(HEADER) -lgcc
	@ printf "$(green)[ok]$(nocolor)\n"
endef

$(foreach source,$(SRC),$(eval		\
	$(call rule-src,$(source)))	\
)




##---
##	Cleaning rules
##---
clean:
	rm -rf $(BUILD)
	rm -rf $(DEBUG)

fclean: clean
	rm -f $(EXEC)

re: fclean all




.PHONY: re fclean clean all install
