#!/usr/bin/sh

# Delete old image
rm -f nop.img

# Build tinyboot
cd boot
  sh build.sh
cd ..

# Build the nop kernel
cd kernel
  sh build.sh
cd ..

# Build libnop
cd libnop
  sh build.sh
cd ..

# Build programs
cd prog
  sh build.sh
cd ..

# Generate the image
mkdir -p mnt
dd if=/dev/zero of=nop.img count=131072
mkfs.fat -R 4 -F32 nop.img
dd conv=notrunc bs=1 count=3 skip=0 seek=0 if=boot/tinyboot.bin of=nop.img
dd conv=notrunc bs=1 count=1920 skip=128 seek=128 if=boot/tinyboot.bin of=nop.img
sudo mount nop.img mnt
sudo cp -R root/* mnt/
sudo cp kernel/nopkernl.bin mnt/
sleep 0.5
sudo umount nop.img

# Clean
rm -rf mnt boot/tinyboot.bin kernel/nopkernl.bin $(find . -name "*.o")
