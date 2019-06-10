#!/usr/bin/make -f
# ---
#	Project: FIXME WRITE DOC !!
#	Author: yann.magnin@epitech.eu
# ---
include global.mk

NAME	:= dump
EXEC	:= $(NAME).g1a
HEADER	:= -Iinclude
DEBUG	:= link_map.txt
LDFLAG	:= -T bootstrap.ld
ICON	:= icon.bmp
BUILD	:= build/


SRC		:=
DIRECTORY	:= src/ $(sort $(dir $(wildcard src/*/*/)))
$(foreach path,$(DIRECTORY),$(eval		\
	SRC	+= $(wildcard $(path)*.c) 	\
			$(wildcard $(path)*.s)	\
			$(wildcard $(path)*.S))	\
)
OBJ	:= $(patsubst src_%,$(BUILD)%.o,$(subst /,_,$(basename $(SRC))))

#all: ;@echo $(SRC) $(OBJ)
all: | $(BUILD) $(EXEC)

install: $(EXEC)
	sudo p7 send $<

$(EXEC): $(OBJ)
	@ printf "$(green)/-------\n/  Link files\n/-------$(nocolor)\n"
	$(CC) -Wl,-M $(LDFLAG) $(CFLAGS) -o $(BUILD)$(NAME).elf $^ $(HEADER) -lgcc > $(DEBUG)
	$(OBJCPY) -R .comment -R .bss -O binary $(BUILD)$(NAME).elf $(BUILD)$(NAME).bin
	$(WRAPPER) $(BUILD)$(NAME).bin -o $@ -i $(ICON)

$(BUILD):
	@ printf "Create $(blue)$@$(nocolor) directory\n"
	@ mkdir -p $@


#
# TODO: find better way to do the job
# Units tests part.
#
tests:
	gcc -std=c11 -Wall -Wno-error=deprecated-declarations -Wno-deprecated-declarations \
	-Werror -D DEBUG -o unit_tests $(HEADER) -I. src/history.c src/string/strtotab.c \
	src/string/atoi_base.c $(wildcard tests/*.c) --coverage -lcriterion
	./unit_tests
	gcovr --exclude tests/ --branches
	rm *.gc*
	rm ./unit_tests



define rule-src
$(patsubst src_%,$(BUILD)%.o,$(subst /,_,$(basename $1))): $1
	@ printf "compiling $(white)$$<$(nocolor)..."
	@ $(CC) $(CFLAGS) -o $$@ -c $$< $(HEADER) -lgcc
	@ printf "$(green)[ok]$(nocolor)\n"
endef

$(foreach source,$(SRC),$(eval		\
	$(call rule-src,$(source)))	\
)


#---
#	Clean rules
#---
clean:
	@ printf "$(red)Delete objects files$(nocolor)\n"
	rm -rf $(BUILD)
	rm -f *.gc*
	rm -f $(DEBUG)

fclean: clean
	@ printf "$(red)Delete binary files$(nocolor)\n"
	rm -f $(EXEC)
	rm -f $(NAME).bin
	rm -f $(NAME).elf

re: fclean all install




.PHONY: clean re fclean install tests
