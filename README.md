# Vhex Kernel

Vhex is a kernel which provide development and retro ingenierie environment for fx9860g.
The Casio's OS code is not open-source, but we can read the binary for understand his working.
There are in particular the syscalls, OS's functions that can be called as a library by the add-ins, which are a gold mine of clues about the functioning of the material.

## features
* Bootloader
* Loader ELF
* Scheduler (not prehemptive yet)
* Driver (EEPROM (read), screen, keyboard, timers)

## Building

Before compiling for the first time, There are few dependencies:
* A suitable GCC toolchain in the `PATH`. You can absolutely *not* build Vhex
  with your system compiler! `sh3eb-elf` is strongly advised.
* [g1a wrapper](https://bitbucket.org/Lephenixnoir/add-in-wrapper/src/master/) to generate the Casio header.
* [P7](https://p7.planet-casio.com/) communication utilities.

To compile the project, you just plug the calculator and execute:

	% make
	% make install
