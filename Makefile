CC=gcc
CFLAGS=-Wall -g -std=c99

SRC=hashmap.c test.c
TestBin=test

test: $(SRC)
	$(CC) -o $@ $(CFLAGS) $(SRC)

clean:
	rm test

.PHONY: clean test
