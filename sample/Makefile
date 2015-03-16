.PHONY: run clean

CPP=g++
CC=gcc
PKG_CONFIG=PKG_CONFIG_PATH="../install/lib/pkgconfig" pkg-config
LIBS=`$(PKG_CONFIG) --libs libds3`
CFLAGS=`$(PKG_CONFIG) --cflags libds3`
CFLAGS+= -g -DDS3_LOG -Wall# Debug flags

all: get-service get-bucket put-bulk

deps:
	./build_local.sh

run-get-service: get-service
	LD_LIBRARY_PATH=../install/lib ./get-service

get-service: get-service.o
	$(CC) $< $(CFLAGS) $(LIBS) -o $@

get-service.o: ../install/lib/pkgconfig/libds3.pc
	$(CC) -c get-service.c $(CFLAGS)

run-get-bucket: get-bucket
	LD_LIBRARY_PATH=../install/lib ./get-bucket

get-bucket: get-bucket.o
	$(CC) $< $(CFLAGS) $(LIBS) -o $@

get-bucket.o: ../install/lib/pkgconfig/libds3.pc
	$(CC) -c get-bucket.c $(CFLAGS)

run-put-bulk: put-bulk
	LD_LIBRARY_PATH=../install/lib ./put-bulk

put-bulk: put-bulk.o
	$(CC) $< $(CFLAGS) $(LIBS) -o $@

put-bulk.o: ../install/lib/pkgconfig/libds3.pc
	$(CC) -c put-bulk.c $(CFLAGS)

../install/lib/pkgconfig/libds3.pc:
	$(error SDK not built. Please run make deps)

clean:
	rm -f *.o get-service get-bucket put-bulk
