#!/usr/bin/sh

# test.nex: hello world program in assembly
nasm test.asm -fbin -O4 -o test.nex
mv test.nex ../root/prog/

# vide.nex: framebuffer video driver
cd vide
  sh build.sh
  mv vide.nex ../../root/prog/
cd ..

# keyb.nex: PS/2 keyboard driver
cd keyb
  sh build.sh
  mv keyb.nex ../../root/prog/
cd ..

# videterm.nex: terminal emulator, using VIDE
cd videterm
  sh build.sh
  mv videterm.nex ../../root/prog/
cd ..

# shel.nex: simple shell, using TERM(videterm, seriterm, etc.)
cd shel
  sh build.sh
  mv shel.nex ../../root/prog/
cd ..
