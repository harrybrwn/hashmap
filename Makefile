CC=gcc
CFLAGS=-Wall -pg -std=c99

SRC=hashmap.c
TestBin=test
Linking=-lpthread

$(TestBin): $(SRC) test.c
	$(CC) -o $@ $(CFLAGS) $(SRC) test.c $(Linking)

clean:
	rm $(TestBin)

proc:
	gcc -E hashmap.c test.c > preproc.i

profile: $(TestBin)
	gprof test gmon.out > prof_out

.PHONY: clean proc profile
