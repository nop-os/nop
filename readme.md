# nop

nop is a simple educational kernel made to be as simple to understand as possible, giving extensive and fun comments in every single file, while being a fully complete kernel.

It should be able to run in any Pentium CPU or better, and it uses a custom bootloader called tinyboot.

This project is made just for fun, and any help would be appreciated, so feel free to help us! (see "Contributions")

## Design

nop doesn't try to be another \*nix operative system, by instead using our own original designs, like our mountlessfs design(designed along with @CaydendW), our own path structure and the use of the UD2 instruction to generate syscalls, as faster instructions like SYSENTER were added in the Pentium II and maintaining compatibility with older CPUs is one of our goals.

As a result of our goal of running on old processors, we are also limited to 32-bit protected mode, although that also makes the design simpler, making it more understandable.

## Why nop?

`nop` is an assembly instruction present on almost every ISA ever designed, and it does absolutely nothing. It can be used as a placeholder for other instructions, for alignment purposes or for timing.

This instruction was just a random choice, it just sounded nice :).

## How to build nop

nop requires a gcc cross-compiler and other depencencies to build it:

```sh
# From nop's directory:

# In Debian/Ubuntu:
sh scripts/install_deps_debian.sh
# In Fedora
sh scripts/install_deps_fedora.sh
# In Arch Linux:
sh scripts/install_deps_arch.sh


sh script/build_gcc.sh
```

Once installed and built, you can start building nop by running make:

```sh
# From nop's directory:

make
```

You can then test it like this:

```sh
# From nop's directory:

# If using QEMU:
make run
# If using bochs
make run-bochs
```

## Contributions

At the time of writing this, nop is being made by a single developer, segfaultdev(me!), although contributions would be greatly appreciated, so if you want to help me, just go ahead and make a PR!

### Contributors

(feel free to add your name here if you contributed)
