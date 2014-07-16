.PHONY: run clean

CPP=g++
CC=gcc
PKG_CONFIG=PKG_CONFIG_PATH="../install/lib/pkgconfig" pkg-config
LIBS=`$(PKG_CONFIG) --libs libds3`
CFLAGS=`$(PKG_CONFIG) --cflags libds3`
CFLAGS+= -g -DDS3_LOG -Wall# Debug flags

all: sample

run: sample
	LD_LIBRARY_PATH=../install/lib ./sample

deps:
	./build_local.sh

sample: main.o
	$(CC) *.o $(CFLAGS) $(LIBS) -o sample

main.o: ../install/lib/pkgconfig/libds3.pc
	$(CC) -c main.c $(CFLAGS)

../install/lib/pkgconfig/libds3.pc:
	$(error SDK not built. Please run make deps)

clean:
	rm -f *.o sample

