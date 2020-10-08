# Vhex Kernel

---

## WARNING

This repository is out-of-date because the main goal as changed since the last push. The original objective of this project was to provide a reverse-engineering environment for the fx9860g calculator (because the SH7305 MPU used by Casio is not documented). But, I have enough information (see my RE notes) to handle correctly the device and provide a real kernel UNIX-like for calculators.

I'm currently reworking the kernel architecture, documentation and the bootloader (which will be called FxBoot, and remove from the current sources). I will update this repository as soon as possible.

You can check my logbook on planet-casio (french): https://www.planet-casio.com/Fr/forums/topic16201-1-vhex-une-plateforme-de-developpement.html \
You can follow the evolution of the projet on Planet-Casio (french): https://www.planet-casio.com/Fr/forums/topic16469-1-projet-vhex-kernel-pour-les-calculatrices-casio.html \
You can read my notes about the SH7305 on Plant-Casio:https://bible.planet-casio.com/yatis

---

## Goal (out-of-date)

Vhex is a kernel which provide reverse engineering environment for the fx9860g calculator because the MPU used by Casio (SH7305) is not documented.

The Casio's OS code is not open-source, but we can read the binary for understand his working. There are in particular syscalls, OS's functions that can be called as a library by the add-ins, which are a gold mine of clues about the functioning of the material.

The goal is to isolate Casio from the device and execute some pieces of Casio's code instruction by instruction using the User Break Controller of the SH7305 (a hardware module that allow to set up hardware break points) and, in parallel, a program that will analyze the TLB and each RAM area available on the device.

---

## Features
* Bootloader (custom, isolate Casio then initialize hardware)
* ELF Loader (shared libraries not supported, see [this GCC issue](https://gcc.gnu.org/legacy-ml/gcc-help/current/000075.html))
* Scheduler (not prehemptive yet)
* Drivers (screen, keyboard, timers, ...)
* Syscall (handle the TRAPA instruction)
* Custom File System on RAM
* Virtual File System (handle Casio's FS and my custom FS)

---

## Building

Before compiling for the first time, There are few dependencies:
* A suitable GCC toolchain in the `PATH`. You can absolutely *not* build Vhex
  with your system compiler! `sh3eb-elf` is strongly advised.
* [g1a wrapper](https://bitbucket.org/Lephenixnoir/add-in-wrapper/src/master/) to generate the Casio header.
* [P7](https://p7.planet-casio.com/) communication utilities.

To compile the project, you just plug the calculator and execute:

	% make
	% make install
