.PHONY: run clean

CPP=g++
CC=gcc
PKG_CONFIG=PKG_CONFIG_PATH="../install/lib/pkgconfig" pkg-config
LIBS=`$(PKG_CONFIG) --libs libds3`
CFLAGS=`$(PKG_CONFIG) --cflags libds3`
CFLAGS+= -g -Wall# Debug flags

all: test 

run: test 
	LD_LIBRARY_PATH=../install/lib ./test

deps:
	./build_local.sh

test: test.o service_tests.o bucket_tests.o
	$(CC) *.o $(CFLAGS) $(LIBS) -o test 

test.o: ../install/lib/pkgconfig/libds3.pc
	$(CC) -c test.c $(CFLAGS)

service_tests.o:
	$(CC) -c service_tests.c $(CFLAGS)

bucket_tests.o:
	$(CC) -c bucket_tests.c $(CFLAGS)

../install/lib/pkgconfig/libds3.pc:
	$(error SDK not built. Please run make deps)

clean:
	rm -f *.o test 

