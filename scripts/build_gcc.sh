#!/bin/sh

# Let's just consider this black magic for now...

export PREFIX="$(pwd)/cross"
export TARGET=i586-elf
export PATH="$PREFIX/bin:$PATH"
export GCC_BUILD_FLAGS="--disable-multilib --disable-nls --enable-languages=c --without-headers"

mkdir -p cross/bin

cd cross
  wget https://ftp.gnu.org/gnu/binutils/binutils-2.36.tar.xz -O binutils-2.36.tar.xz
  tar -xvf binutils-2.36.tar.xz
  mkdir -p build/binutils

  cd build/binutils
    ../../binutils-2.36/configure --target=$TARGET --prefix="$PREFIX" --with-sysroot --disable-nls --disable-werror
    make -j5
    make install -j5
  cd ..

  wget https://ftp.gnu.org/gnu/gcc/gcc-10.3.0/gcc-10.3.0.tar.xz -O gcc-10.3.0.tar.xz
  tar -xvf gcc-10.3.0.tar.xz
  mkdir -p build/gcc

  cd build/gcc
    ../../gcc-10.3.0/configure --target=$TARGET --prefix="$PREFIX" $GCC_BUILD_FLAGS
    make all-gcc -j5
    make all-target-libgcc -j5
    make install-gcc -j5
    make install-target-libgcc -j5
  cd ..
cd ..
