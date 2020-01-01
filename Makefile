#!/usr/bin/make -f
## ---
##	Project: Vhex - On-calc debugger
##	Author: yann.magnin@epitech.eu
## --- 


KERNEL	:= output/vhex.g1a
USER	:= output/shell.elf


##---
##	General rules
##---
all:
	@ make --no-print-directory -C src/lib
	@ make --no-print-directory -C src/kernel
	@ make --no-print-directory -C src/user

install: all
	sudo p7 send --force --no-term $(KERNEL)
	sudo p7 send --force --directory=VHEX $(USER)




##---
##	Cleaning rules
##---
clean:
	make clean --no-print-directory -C src/lib
	make clean --no-print-directory -C src/kernel
	make clean --no-print-directory -C src/user

fclean: clean
	make fclean --no-print-directory -C src/lib
	make fclean --no-print-directory -C src/kernel
	make fclean --no-print-directory -C src/user

re: fclean all




.PHONY: re fclean clean all install
