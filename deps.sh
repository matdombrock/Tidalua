#!/bin/bash
set -e

function lua() {
  target=lua-5.4.7
  curl -L -R -O https://www.lua.org/ftp/$target.tar.gz
  tar zxf $target.tar.gz
  rm -rf $target.tar.gz
  cd $target
  make all test
  cd -
  mv $target ./lua
}

function portaudio() {
  rm -rf portaudio
  target=pa_stable_v190700_20210406
  curl -L -R -O https://files.portaudio.com/archives/$target.tgz
  tar zxf $target.tgz # unpacks to ./portaudio
  rm -rf $target.tgz
  cd portaudio
  cmake -DCMAKE_BUILD_TYPE=Release -DBUILD_SHARED_LIBS=OFF .
  make all
}

lua
portaudio
