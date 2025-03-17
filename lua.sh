#!/bin/bash
target=lua-5.4.7
curl -L -R -O https://www.lua.org/ftp/$target.tar.gz
tar zxf $target.tar.gz
rm -rf $target.tar.gz
cd $target
make all test
cd -
mv $target ./lua
