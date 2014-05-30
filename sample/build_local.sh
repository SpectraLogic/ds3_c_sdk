#!/bin/sh

cd ..

[ ! -f configure ] && autoreconf --install
./configure --prefix=`pwd`/install
make install

cd sample

