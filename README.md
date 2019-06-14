# Vhex disassembler

Vhex is a GUI, Vim based, disassembler for fx9860g.
The Casio's OS code is not open-source but we can read the binary for understand his working.
There are in particular the syscalls, OS's functions that can be called as a library since the add-ins, which are a gold mine of clues about the functioning of the material.

![](https://framapic.org/kMVBl2hbDXOo/PBd7D1FR0LF2.gif)

## features

* All SH3 instructions is traduced "on-the-fly".
* Can handle 3 modes for movements.
* Can handle commands.
* Commands history.
* Compatible with SH3 and SH4 architecture.

List of currently supported commands:

* `systab` jump to the syscall entry (Casio do not use `TRAPA` instruction, just jump into constant address).
* `vbrjmp int` jump to the Casio interrupt handler.
* `vbrjmp except` jump to the Casio exception handler.
* `vbrjmp fault` jump to the Casio memory fault handlers.
* `syscall <id>` jump to the syscall function.
* `ram <zone>` jump to the ram mapped in zone (p0,p1,p2,p3,p4).
* `jmp <address>` jump to the address.
* `rom` jump to the start of the ROM.

List of currently supported modes:

TODO WRITE DOC
* `UNUSED`
* `NORMAL`
* `COMMAND`

## Building

Before compiling for the first time, There are a few dependencies:
* A suitable GCC toolchain in the `PATH`. You can absolutely *not* build Vhex
  with your system compiler!`sh3eb-elf` is strongly advised.
* [g1a wrapper](https://bitbucket.org/Lephenixnoir/add-in-wrapper/src/master/) to generate the Casio header.
* [P7](https://p7.planet-casio.com/) communication utilities.

To compile the project, you just plug the calculator and execute:

	% make
	% make install
