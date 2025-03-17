#!/bin/bash
set -e
rm -rf portaudio
target=pa_stable_v190700_20210406
curl -L -R -O https://files.portaudio.com/archives/$target.tgz
tar zxf $target.tgz # unpacks to ./portaudio
rm -rf $target.tgz
cd portaudio
cmake .
make all
