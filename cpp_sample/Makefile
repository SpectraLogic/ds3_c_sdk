.PHONY: run clean

CPP=g++
PKG_CONFIG=PKG_CONFIG_PATH="../install/lib/pkgconfig" pkg-config
LIBS=`$(PKG_CONFIG) --libs libds3`
CPPFLAGS=`$(PKG_CONFIG) --cflags libds3`
CPPFLAGS+= -g -DDS3_LOG -Wall# Debug flags

all: sample

run: sample
	LD_LIBRARY_PATH=../install/lib ./cpp_sample

deps:
	./build_local.sh

sample: main.o
	$(CPP) *.o $(CPPFLAGS) $(LIBS) -o cpp_sample

main.o: ../install/lib/pkgconfig/libds3.pc
	$(CPP) -c main.cpp $(CPPFLAGS)

../install/lib/pkgconfig/libds3.pc:
	$(error SDK not built. Please run make deps)

clean:
	rm -f *.o cpp_sample

