#!/usr/bin/make -f
## ---
##	Project: Vhex - On-calc debugger
##	Author: yann.magnin@epitech.eu
## ---

##---
##	Build configuration
##---
CONFIG := gcc.cfg
ifeq "$(wildcard $(CONFIG))" ""
$(error "config file $(CONFIG) does not exist")
endif
include $(CONFIG)


##---
##	Static variables
##--
header 		:= -I../../../include/lib -I../../../include/user/shell
build		:= ../../../build/user/shell/$(CONFIG.ARCH)
debug		:= ../../../debug_bin/$(CONFIG.ARCH)
output		:= ../../../output

name		:= shell
EXEC		:= $(output)/$(name)

cflags		:= -Werror -Wall -W -Wextra -std=c11

# Add arch specific flags
ifeq "$(CONFIG.ARCH)" "casio"
EXEC		:= $(output)/$(name).elf
dflags		:= -Wl,-M -T $(name).ld
lflags		:= -L../../lib -llibc -lgcc
cflags		+= $(CONFIG.CEXTRA) -pie
memmap		:= > $(debug)/$(name).map
endif

# Tools
cc 		:= $(CONFIG.TOOLCHAIN)gcc
ld 		:= $(CONFIG.TOOLCHAIN)ld
objcopy 	:= $(CONFIG.TOOLCHAIN)objcpy
objdump 	:= $(CONFIG.TOOLCHAIN)objdump


# Colors
red		:= \033[1;31m
green		:= \033[1;32m
blue		:= \033[1;34m
white		:= \033[1;37m
nocolor		:= \033[1;0m


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
OBJ	:= $(patsubst ._%,$(build)/%.o,$(subst /,_,$(subst src/,,$(basename $(SRC)))))




##---
##	Building rules
##---
all: $(EXEC)

$(EXEC): $(OBJ) | $(debug)
	$(cc) $(dflags) $(cflags) -o $@ $(OBJ) $(header) $(lflags) $(memmap)

$(build) $(debug):
	@ printf "Create $(blue)$@$(nocolor) directory\n"
	@ mkdir -p $@




##---
##	Install / tests rules 
##---
# Install is only available for CASIO arch
ifeq "$(CONFIG.ARCH)" "casio"
install: $(EXEC)
	sudo p7 send --force -d VHEX $^
else
install: $(EXEC)
	valgrind $(EXEC)
endif



##---
##	Debug rules
##---
check:
	@ echo 'src: $(SRC)'
	@ echo 'obj: $(OBJ)'
	@ echo 'directory: $(DIRECTORY)'
	@ echo 'clfags: $(cflags)'
	@ echo 'cc: $(cc)'

asm:
	@ $(objdump) -D $(EXEC) | less

map:
	@ cat $(memmap) | less

sec:
	@ $(objdump) -h $(EXEC)




##---
##	  Automated rules
##---
define rule-src
$(patsubst ._%,$(build)/%.o,$(subst /,_,$(subst src/,,$(basename $1)))): $1 | $(build)
	@ printf "compiling $(white)$$<$(nocolor)..."
	@ $(cc) $(cflags) -o $$@ -c $$< $(header) -lgcc
	@ printf "$(green)[ok]$(nocolor)\n"
endef

$(foreach source,$(SRC),$(eval		\
	$(call rule-src,$(source)))	\
)




##---
##	Cleaning rules
##---
clean:
	rm -rf $(build)
	rm -rf $(debug)

fclean: clean
	rm -f $(EXEC)

re: fclean all




.PHONY: re fclean clean all install
