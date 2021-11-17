CFLAGS=-Wall -std=c99 -pedantic -g
LDLIBS=-lm

all: main clean

main: main.o imagen.o
	gcc $(CFLAGS) $(LDLIBS) main.o imagen.o -o main

main.o: main.c imagen.h vector.h
	gcc $(CFLAGS) -c main.c

imagen.o: imagen.c imagen.h
	gcc $(CFLAGS) -c imagen.c

clean:
	rm -vf *.o
