#!/usr/bin/sh

clang -Os --target=i586-unknown-elf -march=i586 -ffreestanding -fno-builtin -nostdlib -I include -I ../../libnop/include $(find . -name "*.c") -c
find . -name "*.asm" -exec nasm -O4 -felf {} \;

i586-elf-ld -T linker.ld -nostdlib $(find . -name "*.o") $(find ../../libnop -name "*.o") -o vide.nex
rm -rf *.o
