# nop
The toy OS no one asked for!

## About
`nop` is my own attempt at writing a "usable" operative system in C, containing
all the necessary utilities for its usage. This is not by any means a
well-designed OS, but it experiments with some wild and unique concepts like
user-made call handlers.

## Building
First, you have to build the toolkit, consisting of tcc and vlink. Building it
is as easy as doing:

```sh
sh tools.sh
```

Afterwards, to build nop, just run:

```sh
sh build.sh
```

## Running
Once you have built nop, you should have an image called `nop.img` in the nop
directory. You can start nop in a VM by doing:

```sh
sh run.sh # QEMU, or
bochs     # bochs
```

## License
nop is licensed with the MIT license, check `LICENSE` for more details.
