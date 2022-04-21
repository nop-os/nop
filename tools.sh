#!/usr/bin/sh

rm -rf cross

mkdir cross

git clone https://github.com/nop-os/tinycc

cd tinycc
  ./configure --cpu=i586 --extra-cflags="-Wall -O3 -s" --prefix=$(dirname $(pwd))/cross --enable-static --disable-rpath --sysincludepaths=$(dirname $(pwd))/libc/include:$(dirname $(pwd))/cross/lib/tcc/include
  
  make cross-i386
  make install
cd ..

rm -rf tinycc
git clone https://github.com/nop-os/vlink

cd vlink
  mkdir objects
  make
  
  cp vlink ../cross/bin/
cd ..

rm -rf vlink
