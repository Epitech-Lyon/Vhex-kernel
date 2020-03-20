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
BUILD-STATIC		:= ../../build/lib/static
BUILD-DYNAMIC		:= ../../build/lib/dynamic
TARGET-MODULES		:= libc display
DEBUG-DYNLIB		:= ../../debug_bin




#-------		--------#
#	Automated variables	#
#-------		--------#
$(foreach mod,$(TARGET-MODULES),$(eval 									\
	target-$(mod)-dir 		:= $(shell find $(mod) -type d)					$n\
	target-$(mod)-src		:= 								$n\
	$$(foreach path,$$(target-$(mod)-dir),$$(eval							$$n\
		target-$(mod)-src		+= $$$$(wildcard $$(path)/*.c)				$$n\
		target-$(mod)-src		+= $$$$(wildcard $$(path)/*.s)				$$n\
		target-$(mod)-src		+= $$$$(wildcard $$(path)/*.S)				$$n\
	))												$n\
	target-$(mod)-obj		:= $$(subst /,_,$$(basename $$(target-$(mod)-src)))		$n\
	target-$(mod)-obj-static	:= $$(patsubst %,$(BUILD-STATIC)/%.o,$$(target-$(mod)-obj))	$n\
	target-$(mod)-obj-dynamic	:= $$(patsubst %,$(BUILD-DYNAMIC)/%.o,$$(target-$(mod)-obj))	$n\
))
TARGET-LIBS-STATIC	:= $(patsubst %,lib%.a,$(TARGET-MODULES))
TARGET-LIBS-DYNAMIC	:= $(patsubst %,lib%_dyn.so,$(TARGET-MODULES))




#-------		--------#
#	Generic rules		#
#-------		--------#
all: $(TARGET-LIBS-STATIC) $(TARGET-LIBS-DYNAMIC)

$(BUILD-STATIC) $(BUILD-DYNAMIC):
	@ printf "Create $(blue)$@$(nocolor) directory\n"
	@ mkdir -p $@


#-------		--------#
#	Rule functions		#
#-------		--------#
define rule-src-static
$(patsubst %,$(BUILD-STATIC)/%.o,$(subst /,_,$(basename $1))): $1 | $(BUILD-STATIC)
	@ printf "compiling $(white)$$<$(nocolor)..."
	@ $(CC) $(CFLAGS) -c $$< -o $$@ $(HEADER)
	@ printf "$(green)[ok]$(nocolor)\n"
endef
define rule-link-static
$(patsubst %,lib%.a,$1): $2
	@ printf "Link $(green)$$@$(nocolor) lib\n"
	$(AR) crs $$@ $$^
endef

define rule-src-dynamic
$(patsubst %,$(BUILD-DYNAMIC)/%.o,$(subst /,_,$(basename $1))): $1 | $(BUILD-DYNAMIC) 
	@ printf "compiling $(white)$$<$(nocolor)..."
	@ $(CC) -fPIC $(CFLAGS) -c $$< -o $$@ $(HEADER)
	@ printf "$(green)[ok]$(nocolor)\n"
endef
define rule-link-dynamic
$(patsubst %,lib%_dyn.so,$1): $2 | $(DEBUG-DYNLIB)
	@ printf "Link $(green)$$@$(nocolor) lib\n"
	$(CC) -shared -o $$@ $$^ -nostdlib -lgcc
endef



#-------		--------#
#	Automated rules		#
#-------		--------#
$(foreach mod,$(TARGET-MODULES),						\
	$(foreach source,$(target-$(mod)-src),					\
		$(eval $(call rule-src-static,$(source))))			\
	$(eval $(call rule-link-static,$(mod),$(target-$(mod)-obj-static)))	\
)

$(foreach mod,$(TARGET-MODULES), 						\
	$(foreach source,$(target-$(mod)-src),					\
		$(eval $(call rule-src-dynamic,$(source))))			\
	$(eval $(call rule-link-dynamic,$(mod),$(target-$(mod)-obj-dynamic)))	\
)



#-------	--------#
#	Clean rules	#
#-------	--------#
clean:
	rm -rf $(BUILD-STATIC)
	rm -rf $(BUILD-DYNAMIC)

fclean: clean
	rm -f $(TARGET-LIBS-STATIC)
	rm -f $(TARGET-LIBS-DYNAMIC)

re: fclean all




.PHONY: re clean fclean all debug
