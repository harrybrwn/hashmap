CC=gcc
CFLAGS=-Wall -g -std=c99

SRC=hashmap.c
TestBin=test

$(TestBin): $(SRC) test.c
	$(CC) -o $@ $(CFLAGS) $(SRC) test.c

clean:
	rm $(TestBin)

.PHONY: clean
