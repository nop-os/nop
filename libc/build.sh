#!/usr/bin/sh

find . -name "*.c" -exec ../cross/bin/i386-tcc -static -Wall -O1 -s -Iinclude {} -c -o {}.o \;
find . -name "*.asm" -exec nasm -O4 -felf {} -o {}.o \;

# NOTE: using elf here is stupid af, we should be using a.out so we can use the file inside the os itself!
../cross/bin/vlink -b elf32i386 $(find . -name "*.o") -r -o libc.a
cp libc.a ../cross/lib/

rm $(find . -name "*.o")
