CFLAGS=-std=c99 -g -march=native -flto -Wall -Wextra -pedantic -O2

all: find cicg main

cicg: cicg.c icg.o
	$(CC) $(CFLAGS) -o cicg cicg.c icg.o
find: find.c icg.o
	$(CC) $(CFLAGS) -o find find.c icg.o
main: icg.c icg.h
	$(CC) $(CFLAGS) -DTEST -o main icg.c
icg.o: icg.c icg.h
	$(CC) $(CFLAGS) -c -o icg.o icg.c

clean:
	rm *.o
