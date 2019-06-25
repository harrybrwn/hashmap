CC=gcc
CFLAGS=-Wall -g -std=c99

SRC=hashmap.c
TestBin=test
Linking=-lpthread

$(TestBin): $(SRC) test.c
	$(CC) -o $@ $(CFLAGS) $(SRC) test.c $(Linking)

clean:
	rm $(TestBin)

proc:
	gcc -E hashmap.c test.c > preproc.i

.PHONY: clean proc
