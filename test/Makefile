.PHONY: run clean

CPP=g++
PKG_CONFIG=PKG_CONFIG_PATH="../install/lib/pkgconfig" pkg-config
LIBS=`$(PKG_CONFIG) --libs libds3`
CFLAGS=`$(PKG_CONFIG) --cflags libds3`
CFLAGS+= -g -Wall# Debug flags
CFLAGS+= -DBOOST_TEST_DYN_LINK -lboost_unit_test_framework

all: test

mem: test
	LD_LIBRARY_PATH=../install/lib valgrind --leak-check=full --track-origins=yes ./test

debug: test
	LD_LIBRARY_PATH=../install/lib gdb ./test

run: test
	LD_LIBRARY_PATH=../install/lib ./test --catch_system_errors=no

deps:
	./build_local.sh

test: test.o service_tests.o bucket_tests.o
	$(CPP) *.o $(CFLAGS) $(LIBS) -o test

test.o: ../install/lib/pkgconfig/libds3.pc
	$(CPP) -c test.cpp $(CFLAGS)

service_tests.o:
	$(CPP) -c service_tests.cpp $(CFLAGS)

bucket_tests.o:
	$(CPP) -c bucket_tests.cpp $(CFLAGS)

../install/lib/pkgconfig/libds3.pc:
	$(error SDK not built. Please run make deps)

clean:
	rm -f *.o test
