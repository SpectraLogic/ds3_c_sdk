.PHONY: run clean

PKG_CONFIG=PKG_CONFIG_PATH="../install/lib/pkgconfig" pkg-config

ifdef LD_LIB_PATH
LD_PATH=$(LD_LIB_PATH):../install/lib
else
LD_PATH=../install/lib
endif

UNAME_S := $(shell uname -s)
ifeq ($(UNAME_S),Darwin)
LIB_PATH=DYLD_FALLBACK_LIBRARY_PATH=$(LD_PATH)
GDB=lldb
else
LIB_PATH=LD_LIBRARY_PATH=$(LD_PATH)
GDB=gdb
endif

CPP=g++
LIBS=`$(PKG_CONFIG) --libs libds3 glib-2.0`
CFLAGS=`$(PKG_CONFIG) --cflags libds3`
CFLAGS+= -g -Wall# Debug flags
CFLAGS+= -DBOOST_TEST_DYN_LINK -lboost_unit_test_framework

all: test

mem: test
	$(LIB_PATH) valgrind --leak-check=full --track-origins=yes --show-leak-kinds=all ./test

debug: test
	$(LIB_PATH) $(GDB) ./test

run: test
	$(LIB_PATH) ./test --catch_system_errors=no

run_single: test
	$(LIB_PATH) ./test --catch_system_errors=no --run_test=$(test_name)

deps:
	./build_local.sh

test: test.o get_physical_placement.o search_tests.o metadata_tests.o negative_tests.o checksum.o service_tests.o bucket_tests.o multimap_tests.o deletes_test.o job_tests.o get_object.o bulk_get.o
	$(CPP) *.o $(CFLAGS) $(LIBS) -o test

test.o: ../install/lib/pkgconfig/libds3.pc
	$(CPP) -c test.cpp $(CFLAGS)

metadata_tests.o:
	$(CPP) -c metadata_tests.cpp $(CFLAGS)

service_tests.o:
	$(CPP) -c service_tests.cpp $(CFLAGS)

search_tests.o:
	$(CPP) -c search_tests.cpp $(CFLAGS)

bucket_tests.o:
	$(CPP) -c bucket_tests.cpp $(CFLAGS)

deletes_test.o:
	$(CPP) -c deletes_test.cpp $(CFLAGS)

job_tests.o:
	$(CPP) -c job_tests.cpp $(CFLAGS)

bulk_get.o:
	$(CPP) -c bulk_get.cpp $(CFLAGS)

get_object.o:
	$(CPP) -c get_object.cpp $(CFLAGS)

get_physical_placement.o:
	$(CPP) -c get_physical_placement.cpp $(CFLAGS)

negative_tests.o:
	$(CPP) -c negative_tests.cpp $(CFLAGS)

multimap_tests.o:
	$(CPP) -c multimap_tests.cpp $(CFLAGS)

checksum.o:
	$(CPP) -c checksum.cpp $(CFLAGS)

../install/lib/pkgconfig/libds3.pc:
	$(error SDK not built. Please run make deps)

clean:
	rm -f *.o test
