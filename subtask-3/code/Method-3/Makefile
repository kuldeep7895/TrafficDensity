CC=g++

CFLAGS=-c -Wall

OBJS=main.o mouse.o functions.h

PKG=`pkg-config --cflags --libs opencv`

all: cam

cam: main.o mouse.o
	$(CC) $(OBJS) -o cam -pthread $(PKG)

main.o: main.cpp
	$(CC) $(CFLAGS) main.cpp -pthread $(PKG)
	
mouse.o: mouse.cpp
	$(CC) $(CFLAGS) mouse.cpp -pthread $(PKG)
	
clean:
	rm -rf *o cam
