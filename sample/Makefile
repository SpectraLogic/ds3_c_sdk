CPP=g++
LIBS=`pkg-config --libs libds3`
CFLAGS=`pkg-config --cflags libds3`
CFLAGS+= -g -DDS3_LOG -Wall# Debug flags

all: main.o
	$(CPP) *.o $(CFLAGS) $(LIBS) -o sample

main.o:
	$(CPP) -c main.cpp $(CFLAGS)

clean:
	rm *.o sample

