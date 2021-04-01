CC=g++

CFLAGS=-c -Wall

OBJS=main.o mouse.o functions.h

PKG=`pkg-config --cflags --libs opencv`

all: cam

cam: main.o mouse.o
	$(CC) $(OBJS) -o cam $(PKG)

main.o: main.cpp
	$(CC) $(CFLAGS) main.cpp $(PKG)
	
mouse.o: mouse.cpp
	$(CC) $(CFLAGS) mouse.cpp $(PKG)
	
clean:
	rm -rf *o cam
