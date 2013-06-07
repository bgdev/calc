CFLAGS = -Wall -Wextra -pedantic -g

all: main test

main: main.o calc.o
	$(CC) $(CFLAGS) -o calc calc.o main.o

calc.o: calc.h
main.o: calc.h
test.o: calc.h

test: test.o calc.o
	$(CC) $(CFLAGS) -o test calc.o test.o

.PHONY: check-syntax
check-syntax: all
