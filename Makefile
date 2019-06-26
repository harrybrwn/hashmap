CC=gcc
CFLAGS=-Wall -std=c99 -pg

SRC=hashmap.c
Test=tests/test
Example=tests/doc_example

all: $(Test) $(Example)

test: all
	@./$(Test)
	@./$(Example) > /dev/null

.PHONY: all test

$(Test): $(SRC) $(Test).c
	$(CC) -o $@ $(CFLAGS) $^

$(Example): $(SRC) $(Example).c
	$(CC) -o $@ $(CFLAGS) $^

clean:
	rm $(Test) $(Example)

proc:
	gcc -E hashmap.c test.c > preproc.i

profile: $(TestBin)
	$(CC) -o test -std=c99 -pg $(SRC) test.c
	gprof ./test gmon.out > prof_out

.PHONY: clean proc profile
