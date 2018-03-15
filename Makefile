# Simple makefile that only builds the simulator (for now)
CC=gcc
CFLAGS=-std=c99 -pipe -w

all: simulator
	$(CC) $(CFLAGS) simulator.o -o simulator

simulator: simulator.c
	$(CC) $(CFLAGS) -c simulator.c

clean:
	rm *.o simulator
