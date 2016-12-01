all: main.c
	gcc main.c -g -O2 -std=c99 -Wall -Wextra -pedantic -o icg
