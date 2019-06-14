# Vhex disassembler

Vhex is a GUI, Vim based, disassembler for fx9860g.
The Casio's OS code is not open-source, but we can read the binary for understand his working.
There are in particular the syscalls, OS's functions that can be called as a library since the add-ins, which are a gold mine of clues about the functioning of the material.

![](https://framapic.org/kMVBl2hbDXOo/PBd7D1FR0LF2.gif)

## features

* All SH3 instructions traduced "on-the-fly".
* Can handle 3 modes for movements.
* Can handle commands.
* Commands history.
* Compatible with SH3 and SH4 architecture.

### List of currently supported commands:

* `systab`		jump into the Casio "syscall handler".
* `vbrjmp <zone>`	jump into the Casio interrupt, exception or tlb miss handlers.
* `syscall <id>`	jump into the syscall function.
* `ram <zone>`		jump into the RAM mapped at different parts of the Virtual Memory.
* `jmp <address>`	jump where you want.
* `rom <zone>`		jump into the ROM mapped at different parts of the Virtual Memory.
* `help <cmd>`		display command's help.

### List of currently supported modes:

* `UNUSED`	The session is unused.
* `NORMAL`	User can move freely where it wants.
* `COMMAND`	User can type some commands.

## Building

Before compiling for the first time, There are few dependencies:
* A suitable GCC toolchain in the `PATH`. You can absolutely *not* build Vhex
  with your system compiler! `sh3eb-elf` is strongly advised.
* [g1a wrapper](https://bitbucket.org/Lephenixnoir/add-in-wrapper/src/master/) to generate the Casio header.
* [P7](https://p7.planet-casio.com/) communication utilities.

To compile the project, you just plug the calculator and execute:

	% make
	% make install
