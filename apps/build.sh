#!/usr/bin/sh

nasm init.asm -fbin -O4 -o init.nex
mv init.nex ../root/syst/
