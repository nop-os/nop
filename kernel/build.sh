#!/usr/bin/sh

clang --target=i586-unknown-elf -march=i586 -ffreestanding -fno-builtin -nostdlib -I include $(find . -name "*.c") -c
nasm -felf $(find . -name "*.asm")
i586-elf-ld -T linker.ld -nostdlib $(find . -name "*.o") -o nopkernl.bin