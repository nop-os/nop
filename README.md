# nop

nop is a simple educational kernel made to be as small and simple to understand as possible, while being a fully complete kernel.

It should be able to run in any Pentium CPU or better, and it uses a custom bootloader called tinyboot.

This project is made just for fun, and any help would be appreciated, so feel free to help us! (see "Contributions")

## Design

nop doesn't try to be another \*nix operative system, by instead using our own original designs, our own path structure and the use of the UD2 instruction to generate syscalls, as faster options were not present in original Pentium CPUs.

As a result of our goal of running on old processors, we are also limited to 32-bit protected mode, although that also makes the design simpler, making it more understandable.

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

qemu-system-i386 -hda nop.img -m 64M
```

## TODO

(keep in mind nop has recently been restarted from scratch as the codebase was a bit too messy)

- [x] PMM
- [x] VMM
- [x] Kernel heap allocator
- [ ] IDT
- [ ] NMI
- [x] Connection API
- [x] Serial port connector
- [ ] Framebuffer connector
- [ ] ATA PIO connector
- [ ] PCI connector
- [ ] NVME connector
- [ ] Soundblaster 16 connector
- [ ] Multitasking
- [ ] ELF loading
- [ ] Simple "userland" shell

## License

This project uses the MIT license, as this is just an educational project for people to learn from.

## Contributions

At the time of writing this, nop is being made by a single developer, segfaultdev(me!), although contributions would be greatly appreciated, so if you want to help me, just go ahead and make a PR!

### Contributors

(feel free to add your name here if you contributed)
