#!/bin/sh

cd ..

[ ! -f configure ] && autoreconf --install
#CFLAGS="-g -O0" ./configure --prefix=`pwd`/install
./configure --prefix=`pwd`/install
make install

cd sample

