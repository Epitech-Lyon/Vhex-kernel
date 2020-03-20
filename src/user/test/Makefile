#!/usr/bin/make -f
## ---
##	Project: Vhex - On-calc debugger
##	Author: yann.magnin@epitech.eu
## ---
include ../../../global.mk

##---
##	Static variables
##--
HEADER		:= -I../../../include -I../../../include/user
LIBS		:= -L../../lib/ -lunistd -lstdio -lstring -ldisplay -lgcc
BUILD		:= ../../../build/user/test
DEBUG		:= ../../../debug_bin
OUTPUT		:= ../../../output

NAME		:= test
EXEC		:= $(OUTPUT)/$(NAME).elf
LDFLAG		:= -T $(NAME).ld
MEMORY_MAP	:= $(DEBUG)/$(NAME).map




##---
##	Automated variables
##---
SRC		:=
DIRECTORY	:= $(shell find . -not -path "*/\.*" -type d)
# Get all source files
$(foreach path,$(DIRECTORY),$(eval		\
	SRC	+= $(wildcard $(path)/*.c)	\
			$(wildcard $(path)/*.S)	\
			$(wildcard $(path)/*.s)	\
))
# Geneate all object files
OBJ	:= $(patsubst ._%,$(BUILD)/%.o,$(subst /,_,$(subst src/,,$(basename $(SRC)))))




##---
##	General rules
##---
all: $(EXEC)

$(EXEC): $(OBJ) | $(DEBUG)
	$(CC) -pie -Wl,-M $(LDFLAG) $(CFLAGS) -o $@ $(OBJ) $(HEADER) $(LIBS) > $(MEMORY_MAP)

$(BUILD) $(DEBUG):
	@ printf "Create $(blue)$@$(nocolor) directory\n"
	@ mkdir -p $@

install: $(EXEC)
	sudo p7 send --force -d VHEX $^

check:
	@ echo 'src: $(SRC)'
	@ echo 'obj: $(OBJ)'
	@ echo 'directory: $(DIRECTORY)'

asm:
	@ $(OBJDUMP) -D $(EXEC) | less

map:
	@ cat $(MEMORY_MAP) | less

sec:
	@ $(OBJDUMP) -h $(EXEC)


##---
##	  Automated rules
##---
define rule-src
$(patsubst ._%,$(BUILD)/%.o,$(subst /,_,$(subst src/,,$(basename $1)))): $1 | $(BUILD) 
	@ printf "compiling $(white)$$<$(nocolor)..."
	@ $(CC) -pie $(CFLAGS) -o $$@ -c $$< $(HEADER) -lgcc
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
