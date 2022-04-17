#!/usr/bin/sh

find . -name "*.c" -exec ../../cross/bin/i386-tcc -m32 -static -Wall -O1 -s -I include {} -c -o {}.o \;
find . -name "*.asm" -exec nasm -O4 -felf {} -o {}.o \;

../../cross/bin/vlink $(find . -name "*.o") -o test.out
cp test.out ../../root/prog/

rm $(find . -name "*.o")
