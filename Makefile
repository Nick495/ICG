all: icg.c
	gcc icg.c -g -O2 -std=c99 -Wall -Wextra -pedantic -o icg
