#!/usr/bin/sh

find . -maxdepth 1 -type d -path "./*" | while read -r path; do
  echo "[nop] building '$path'..."
  
  cd $path
    sh build.sh
  cd ..
done
