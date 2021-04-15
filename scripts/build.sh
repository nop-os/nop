#!/bin/sh

export CC="$HOME/opt/cross/bin/i586-elf-gcc"
export LD="$HOME/opt/cross/bin/i586-elf-ld"

export CC_FLAGS="-Ofast --freestanding -Wall -nostdlib"
export LD_FLAGS="-T resources/linker.ld -nostdlib -pie"

cd src
  nasm -fbin boot/boot.asm -o ../build/boot.bin

  nasm -felf nop_start.asm -o ../build/nop_start.o
  $CC $CC_FLAGS -c nop.c -o ../build/nop.o
cd ..

$LD $LD_FLAGS -o build/nop.bin $(find build -name "*.o")

cp build/boot.bin image/boot/nop.bin
dd if=build/nop.bin of=image/boot/nop.bin seek=0 status=progress conv=notrunc oflag=append

cd image
  flatfs ../nop.img -f boot/config.txt -f boot/nop.bin
cd ..

dd if=resources/tinyboot.bin of=nop.img seek=0 skip=0 status=progress conv=notrunc
