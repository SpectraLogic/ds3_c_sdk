#!/bin/sh

make -C ./sample clean \
    && make maintainer-clean \
    && rm -rf \
        aclocal.m4 \
        ar-lib \
        compile \
        config.guess \
        config.h.in \
        config.sub \
        configure \
        depcomp \
        install-sh \
        ltmain.sh \
        m4 \
        Makefile.in \
        missing \
        install

