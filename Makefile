CC=gcc
CFLAGS=-Wall -std=c99 $(shell pkg-config --cflags raylib)
LDFLAGS=$(shell pkg-config --libs raylib)

all: main

main: main.c
	$(CC) $(CFLAGS) main.c -o main $(LDFLAGS)

clean:
	rm -f main

