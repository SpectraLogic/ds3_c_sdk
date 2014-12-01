.PHONY: run clean

CPP=g++
CC=gcc
PKG_CONFIG=PKG_CONFIG_PATH="../install/lib/pkgconfig" pkg-config
LIBS=`$(PKG_CONFIG) --libs libds3`
CFLAGS=`$(PKG_CONFIG) --cflags libds3`
CFLAGS+= -g -DDS3_LOG -Wall# Debug flags

run-get-service: get-service
	LD_LIBRARY_PATH=../install/lib ./get-service

deps:
	./build_local.sh

get-service: get-service.o
	$(CC) *.o $(CFLAGS) $(LIBS) -o get-service

get-service.o: ../install/lib/pkgconfig/libds3.pc
	$(CC) -c get-service.c $(CFLAGS)

../install/lib/pkgconfig/libds3.pc:
	$(error SDK not built. Please run make deps)

clean:
	rm -f *.o get-service
