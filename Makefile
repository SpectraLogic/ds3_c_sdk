CC=gcc
CFLAGS=`pkg-config --libs --cflags glib-2.0` `curl-config --cflags` \
  `curl-config --libs`
CFLAGS+= -g

all: main.o ds3.o net.o
		$(CC) *.o $(CFLAGS) -o ds3 

main.o:
		$(CC) -c main.c $(CFLAGS) 

ds3.o:
		$(CC) -c ds3.c $(CFLAGS)

net.o:
		$(CC) -c net.c $(CFLAGS)

clean:
		rm *.o ds3 
