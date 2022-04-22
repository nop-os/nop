#!/usr/bin/sh

qemu-system-i386 -hda nop.img -m 32M -device adlib -device ne2k_pci -soundhw pcspk -accel tcg,thread=single -no-reboot
