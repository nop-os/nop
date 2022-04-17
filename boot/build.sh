#!/usr/bin/sh

rm tinyboot.bin
nasm stage_1.asm -fbin -O4 -o tinyboot.bin
