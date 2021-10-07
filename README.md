# nop

nop is a simple educational x86 kernel made to be as small and simple to understand as possible, while being a fully complete kernel.
It should be able to run in any Pentium CPU or better, and it uses a custom bootloader called tinyboot.

This project is made just for fun, and any help would be appreciated, so feel free to help us! (see "Contributions")

## Design

nop could be described as an insecure microkernel, as the kernel is really small and almost all drivers and programs run in userspace, but that userspace is actually running in ring 0, giving access to everything.
It has a very unique syscall and IPC design, merging those two together, and calling syscalls by sending IPC messages to a fake running program with ID 0.

It's not multitasking, but multiple programs can be loaded into memory at the same time, so some level of multitasking can be implemented by setting up event handlers that trigger each 10ms tick in each program.
The kernel runs in 32-bit protected mode, in order to be able to run in older computers.

## Why nop?

`nop` is an assembly instruction present on almost every ISA ever designed, and it does absolutely nothing. It can be used as a placeholder for other instructions, for alignment purposes or for timing.
This instruction was just a random choice, it just sounded nice :).

## How to build nop

nop requires clang for building C code, nasm for assembly and a build of binutils targeting `i586-elf` for linking. Once installed, you can build it like this:

```sh
# From nop's directory:

sh build.sh
```

Once built, you can test it with QEMU like this:

```sh
# From nop's directory:

qemu-system-i386 -hda nop.img -m 64M -serial /dev/tty -device sb16
```

## TODO

- [x] Memory management
- [x] IPC and syscalls
- [x] ATA PIO and FAT32 reading
- [x] Parsing and running programs
- [ ] FAT32 writing

- [x] PS/2 keyboard driver
- [x] Framebuffer driver
- [ ] Shell(not finished yet)

## License

This project uses the nop license, check LICENSE for more information.

## Contributions

At the time of writing this, nop is being made by a single developer, segfaultdev(me!), although contributions would be greatly appreciated, so if you want to help me, just go ahead and make a PR!

### Contributors

(feel free to add your name here if you contributed)
