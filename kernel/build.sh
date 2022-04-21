#!/usr/bin/sh

rm nopkernl.bin nopdebug.bin nopkernl.elf nopdebug.elf

find . -name "*.c" -exec ../cross/bin/i386-tcc -m32 -static -nostdinc -nostdlib -Wall -O1 -Iinclude -I../cross/lib/tcc/include {} -c -o {}.o \;
find . -name "*.asm" -exec nasm -O4 -felf {} -o {}.o \;

../cross/bin/vlink -b elf32i386 -T linker.ld -nostdlib $(find . -name "*.o") -o nopkernl.elf
objcopy -O binary --only-section=.text nopkernl.elf nopkernl.bin # TODO: directly generate both elf and binary and stop using local binutils

cp nopkernl.bin ../root/
rm $(find . -name "*.o")

find . -name "*.c" -exec ../cross/bin/i386-tcc -m32 -static -nostdinc -nostdlib -Wall -O0 -Iinclude -I../cross/lib/tcc/include {} -c -o {}.o -DNOP_DEBUG \;
find . -name "*.asm" -exec nasm -O0 -felf {} -o {}.o \;

../cross/bin/vlink -b elf32i386 -T linker.ld -nostdlib $(find . -name "*.o") -o nopdebug.elf
objcopy -O binary --only-section=.text nopdebug.elf nopdebug.bin # TODO: directly generate both elf and binary and stop using local binutils

cp nopdebug.bin ../root/
rm $(find . -name "*.o")
