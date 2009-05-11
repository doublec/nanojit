CFLAGS=-g -include mozilla-config.h

all: example

example: example.o
	g++ -o example example.o ../libnanojit.a 

example.o: example.cpp
	g++ -c -I.. $(CFLAGS) -o example.o example.cpp

clean:
	rm example.o
	rm example