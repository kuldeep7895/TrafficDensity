CC=g++

CFLAGS=-c -Wall

OBJS=main.o mouse.o queue_density.o dynamic_density.o functions.h

THREAD=-pthread

PKG=`pkg-config --cflags --libs opencv`

all: cam

cam: main.o mouse.o queue_density.o dynamic_density.o
	$(CC) $(OBJS) -o cam $(THREAD) $(PKG)

main.o: main.cpp
	$(CC) $(CFLAGS) main.cpp $(THREAD) $(PKG)
	
mouse.o: mouse.cpp
	$(CC) $(CFLAGS) mouse.cpp $(THREAD) $(PKG)

queue_density.o: queue_density.cpp
	$(CC) $(CFLAGS) queue_density.cpp $(THREAD) $(PKG)
	
dynamic_density.o: dynamic_density.cpp
	$(CC) $(CFLAGS) dynamic_density.cpp $(THREAD) $(PKG)
	
clean:
	rm -rf *o cam
