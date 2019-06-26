CC=gcc
CFLAGS=-Wall -std=c99 -g

SRC=hashmap.c

# Directories
TestDir=tests
LibDir=lib

# Binaries
Test=$(TestDir)/test
Example=$(TestDir)/doc_example
StaticLib=$(LibDir)/libhashmap_s.a
SharedLib=$(LibDir)/libhashmap.so

all: $(Test) $(Example) lib

test: $(Test) $(Example)
	@./$(Test)
	@./$(Example) > /dev/null

.PHONY: all test

$(Test): $(SRC) $(Test).c
	$(CC) -I. -L$(LibDir) -o $@ $(CFLAGS) $^

$(Example): $(SRC) $(Example).c $(StaticLib)
	$(CC) -I. -L$(LibDir) -o $@ $(CFLAGS) $(Example).c -lhashmap_s

lib: $(SharedLib) $(StaticLib)
.PHONY: lib

$(SharedLib): $(SRC)
	@if [ ! -d lib ]; then mkdir lib; fi
	$(CC) $(CFLAGS) -fPIC -shared -o $@ $(SRC) -lc

$(StaticLib): hashmap.c
	@if [ ! -d lib ]; then mkdir lib; fi
	$(CC) -c $(CFLAGS) -o hashmap.o hashmap.c
	ar rcs $@ hashmap.o
	@rm hashmap.o

clean: all
	@if [ -d lib ]; then rm -rf lib; fi
	rm $(Test) $(Example)

proc:
	gcc -E hashmap.c test.c > preproc.i

profile: $(TestBin)
	$(CC) -o test -std=c99 -pg $(SRC) test.c
	gprof ./test gmon.out > prof_out

.PHONY: clean proc profile
