CC=gcc
CFLAGS=-Wall -g -std=c99

SRC=hashmap.c test.c

test: $(SRC)
	$(CC) -o $@ $(CFLAGS) $(SRC)

