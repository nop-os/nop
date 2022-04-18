#!/usr/bin/sh

# delete old image
rm -f nop.img

# build tinyboot
cd boot
  echo "[nop] building tinyboot..."
  sh build.sh
cd ..

# build the nop kernel
cd kernel
  echo "[nop] building nop kernel..."
  sh build.sh
cd ..

# build the programs
cd prog
  echo "[nop] building programs..."
  sh build.sh
cd ..

# make root directories
mkdir -p root/{syst,prog}

# generate the image
echo "[nop] generating image..."
dd if=/dev/zero of=nop.img count=131072
mkfs.fat -s 1 -R 4 -F32 nop.img
mcopy -i nop.img -s root/* ::
dd conv=notrunc bs=1 count=3 skip=0 seek=0 if=boot/tinyboot.bin of=nop.img
dd conv=notrunc bs=1 count=1920 skip=128 seek=128 if=boot/tinyboot.bin of=nop.img
