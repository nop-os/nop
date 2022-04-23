#!/usr/bin/sh

find . -name "*.c" -exec ../../cross/bin/i386-tcc -static -Wall -O1 -s -Iinclude {} -c -o {}.o \;
find . -name "*.asm" -exec nasm -O4 -felf {} -o {}.o \;

../../cross/bin/vlink $(find . -name "*.o") -lc -o dump.out
cp dump.out ../../root/prog/

rm $(find . -name "*.o")
