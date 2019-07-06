CC=gcc
CFLAGS=-Wall -std=c99 -g

SRC=hashmap.c

# Directories
TestDir=tests
LibDir=lib

# Binaries
Test=$(TestDir)/test
Example=$(TestDir)/doc_example
Benchmark=$(TestDir)/benchmarks
TestSRC=tests/test_common.c

StaticLib=$(LibDir)/libhashmapstatic.a
SharedLib=$(LibDir)/libhashmap.so

all: $(Test) $(Example) $(Benchmark) lib

test: $(Test) $(Example) $(Benchmark)
	@./$(Test)
	@./$(Example) > /dev/null
	@./$(Benchmark)

bench: $(Benchmark)
	@./$(Benchmark)

.PHONY: all test bench

$(Test): $(TestSRC) $(SRC) $(Test).c
	$(CC) -I. -L$(LibDir) -o $@ $(CFLAGS) $^

$(Example): $(TestSRC) $(Example).c $(StaticLib)
	$(CC) -I. -L$(LibDir) -o $@ $(CFLAGS) $(TestSRC) $@.c -lhashmapstatic

$(Benchmark): $(TestSRC) $(StaticLib) $(Benchmark).c
	$(CC) -I. -L$(LibDir) -o $@ $(CFLAGS) $(TestSRC) $@.c -lhashmapstatic

lib: $(SharedLib) $(StaticLib)
.PHONY: lib

$(SharedLib): hashmap.c hashmap.h
	@if [ ! -d lib ]; then mkdir lib; fi
	$(CC) $(CFLAGS) -fPIC -shared -o $@ $(SRC) -lc

$(StaticLib): hashmap.c hashmap.h
	@if [ ! -d lib ]; then mkdir lib; fi
	$(CC) -c $(CFLAGS) -o hashmap.o hashmap.c
	ar rcs $@ hashmap.o
	@rm hashmap.o

clean:
	@for file in $(Test) $(Example) $(Benchmark); do\
		if [ -f $$file ]; then\
			rm $$file;\
		fi;\
	done
	@if [ -d lib ]; then rm -rf lib; fi

proc:
	gcc -I. -E hashmap.c $(Test).c > preproc.i

profile: $(TestBin)
	$(CC) -o test -std=c99 -pg $(SRC) test.c
	gprof ./test gmon.out > prof_out

.PHONY: clean proc profile
