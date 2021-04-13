#!/bin/sh

cd src
  nasm -fbin boot/boot.asm -o ../build/boot.bin

  nasm -felf nop.asm -o ../build/nop.o

cd ..

cd image
  flatfs ../nop.img -f boot/config.txt -f boot/nop.bin
cd ..

dd if=resources/tinyboot.bin of=nop.img seek=0 skip=0 status=progress conv=notrunc
