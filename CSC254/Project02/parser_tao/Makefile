# Trivial makefile for the calculator scanner/parser.

# Note that the rule for the goal (parse)
# must be the first one in this file.

CC = g++
CFLAGS = -g -O2 -Wall -Wpedantic

.c.o:
	$(CC) $(CFLAGS) -c $<

parse: parse.o scan.o
	$(CC) $(CFLAGS) -o parse parse.o scan.o

parse.o: scan.h
scan.o: scan.h

clean:
	rm parse.o scan.o parse