CFLAGS=-Wall -std=c99 -pedantic -g
LDLIBS=-lm

all: main

main: main.o imagen.o vector.o
	gcc $(CFLAGS) main.o imagen.o vector.o $(LDLIBS) -o main

main.o: main.c imagen.h vector.h
	gcc $(CFLAGS) $(LDLIBS) -c main.c

imagen.o: imagen.c imagen.h
	gcc $(CFLAGS) -c imagen.c

vector.o: vector.c vector.h
	gcc $(CFLAGS) $(LDLIBS) -c vector.c

clean:
	rm -vf *.o main
