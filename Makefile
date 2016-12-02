CFLAGS=-std=c99 -g -march=native -flto -Wall -Wextra -pedantic -O2

all: find
find: find.c icg.o
	$(CC) $(CFLAGS) -o find find.c icg.o
icg.o: icg.c icg.h
	$(CC) $(CFLAGS) -c -o icg.o icg.c
