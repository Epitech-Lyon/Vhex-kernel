#!/usr/bin/make -f
## ---
##	Project: Vhex - Kernel for fx9860
##	Author: yann.magnin@epitech.eu
## ---
include ../../global.mk




##---
##	Static variables
##--
HEADER		:= -I../../include -I../../include/lib
BUILD		:= ../../build/kernel
OUTPUT		:= ../../output
DEBUG		:= ../../debug_bin

NAME		:= vhex
EXEC		:= $(OUTPUT)/$(NAME).g1a
LDFLAG		:= -T $(NAME).ld
MEMORY_MAP	:= $(DEBUG)/$(NAME).map
LIBS		:= -L../lib -lgcc -llibc -ldisplay
ICON		:= icon.bmp




##---
##	Automated variables
##---
# Get all directory
SRC		:=
DIRECTORY	:= $(shell find . -not -path "*/\.*" -type d)
# Get all source files
$(foreach path,$(DIRECTORY),$(eval		\
	SRC	+= $$(wildcard $(path)/*.c)	$n\
	SRC	+= $$(wildcard $(path)/*.S)	$n\
	SRC	+= $$(wildcard $(path)/*.s)	$n\
))
# Geneate all object files
OBJ	:= $(patsubst ._%,$(BUILD)/%.o,$(subst /,_,$(basename $(SRC))))




##---
##	General rules
##---
all: $(OUTPUT) $(EXEC)

$(EXEC): $(OBJ) | $(DEBUG)
	$(CC) -Wl,-M $(LDFLAG) $(CFLAGS) -o $(DEBUG)/$(NAME).elf $(OBJ) $(HEADER) $(LIBS) > $(MEMORY_MAP)
	$(OBJCOPY) -R .comment -R .bss -O binary $(DEBUG)/$(NAME).elf $(DEBUG)/$(NAME).bin
	$(WRAPPER) $(DEBUG)/$(NAME).bin -o $@ -i $(ICON)

$(BUILD) $(DEBUG) $(OUTPUT):
	@ printf "Create $(blue)$@$(nocolor) directory\n"
	@ mkdir -p $@




##---
##	Debugging rules
##---
debug:
	@ echo 'src: $(SRC)'
	@ echo 'obj: $(OBJ)'
	@ echo 'directory: $(DIRECTORY)'

asm:
	@ $(OBJDUMP) -D $(DEBUG)/$(NAME).elf | less

map:
	@ cat $(MEMORY_MAP) | less

sec:
	@ $(OBJDUMP) -h $(DEBUG)/$(NAME).elf




##---
##	  Automated rules
##---
define rule-src
$(patsubst ._%,$(BUILD)/%.o,$(subst /,_,$(basename $1))): $1 | $(BUILD)
	@ printf "compiling $(white)$$<$(nocolor)..."
	@ $(CC) $(CFLAGS) -o $$@ -c $$< $(HEADER) -lgcc
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




.PHONY: re fclean clean all
.PHONY: debug asm map sec
