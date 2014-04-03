CC=gcc
CFLAGS=`pkg-config --libs --cflags glib-2.0` 

all: main.o ds3.o
		$(CC) *.o $(CFLAGS) -o ds3 

main.o:
		$(CC) -c main.c $(CFLAGS) 

ds3.o:
		$(CC) -c ds3.c $(CFLAGS)

clean:
		rm *.o ds3 
