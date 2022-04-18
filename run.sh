#!/usr/bin/sh

qemu-system-i386 -hda nop.img -m 128M -device adlib -device ne2k_pci -soundhw pcspk -accel tcg,thread=single
