#!/usr/bin/make -f
# ---
#	Project: Vhex - Librairies part.
#	Author: yann.magnin@epitech.eu
# ---
include ../../global.mk




#-------		--------#
#	Generic variables	#
#-------		--------#
HEADER			:= -I../../include
BUILD			:= ../../build/lib/
TARGET-MODULES		:= stdio string unistd display




#-------		--------#
#	Automated variables	#
#-------		--------#
$(foreach mod,$(TARGET-MODULES),$(eval 							\
	target-$(mod)-src	:= $$(wildcard $(mod)/*.c)				$n\
	target-$(mod)-src	+= $$(wildcard $(mod)/*.s)				$n\
	target-$(mod)-src	+= $$(wildcard $(mod)/*.S)				$n\
	target-$(mod)-obj	:= $$(subst /,_,$$(basename $$(target-$(mod)-src)))	$n\
	target-$(mod)-obj	:= $$(patsubst %,$(BUILD)%.o,$$(target-$(mod)-obj))	$n\
))
TARGET-LIBS	:= $(patsubst %,lib%.a,$(TARGET-MODULES))




#-------		--------#
#	Generic rules		#
#-------		--------#
all: $(TARGET-LIBS)

$(BUILD):
	@ printf "Create $(blue)$@$(nocolor) directory\n"
	@ mkdir -p $@

debug:
	@ echo 'lib: $(TARGET-LIBS)'
	@ echo 'src: $(target-stdio-src)'
	@ echo 'obj: $(target-stdio-obj)'
	@ echo 'directory: $(TARGET-MODULES)'
	@ echo 'debug: $(BUILD)'




#-------		--------#
#	Rule functions		#
#-------		--------#
define rule-src
$(patsubst %,$(BUILD)%.o,$(subst /,_,$(basename $1))): $1 | $(BUILD) 
	@ printf "compiling $(white)$$<$(nocolor)..."
	@ $(CC) $(CFLAGS) -c $$< -o $$@ $(HEADER)
	@ printf "$(green)[ok]$(nocolor)\n"
endef
define rule-link
$(patsubst %,lib%.a,$1): $2
	@ printf "Link $(green)$$@$(nocolor) lib\n"
	$(AR) crs $$@ $$^
endef




#-------		--------#
#	Automated rules		#
#-------		--------#
$(foreach mod,$(TARGET-MODULES), 					\
	$(foreach source,$(target-$(mod)-src),				\
		$(eval $(call rule-src,$(source),$(mod))))		\
	$(eval $(call rule-link,$(mod),$(target-$(mod)-obj)))		\
)




#-------	--------#
#	Clean rules	#
#-------	--------#
clean:
	rm -rf $(BUILD)

fclean: clean
	rm -f $(TARGET-LIBS)

re: fclean all




.PHONY: re clean fclean all debug
