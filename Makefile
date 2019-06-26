CC=gcc
CFLAGS=-Wall -std=c99 -pg

SRC=hashmap.c
TestBin=test

$(TestBin): $(SRC) test.c
	$(CC) -o $@ $(CFLAGS) $(SRC) test.c

clean:
	rm $(TestBin)

proc:
	gcc -E hashmap.c test.c > preproc.i

profile: $(TestBin)
	$(CC) -o test -std=c99 -pg $(SRC) test.c
	gprof ./test gmon.out > prof_out

.PHONY: clean proc profile
