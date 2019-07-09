CC=gcc
CFLAGS=-Wall -L$(LibDir) -I. -std=c89 -g
AR=ar

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

test: py-test $(Test) $(Example) $(Benchmark)
	@./$(Test)
	@./$(Example) > /dev/null

bench: $(Benchmark)
	@./$(Benchmark)

.PHONY: all test bench

$(Test): $(TestSRC) $(SRC) $(Test).c
	$(CC) -DHASHMAP_TESTING $(CFLAGS) -o $@ $^

$(Example): $(TestSRC) $(Example).c $(StaticLib)
	$(CC) -o $@ $(CFLAGS) $(TestSRC) $@.c -lhashmapstatic

$(Benchmark): $(TestSRC) $(StaticLib) $(Benchmark).c
	$(CC) -DHASHMAP_TESTING -o $@ $(CFLAGS) $(TestSRC) $@.c -lhashmapstatic

lib: $(SharedLib) $(StaticLib)
.PHONY: lib

$(SharedLib): hashmap.c hashmap.h
	@if [ ! -d lib ]; then mkdir lib; fi
	$(CC) $(CFLAGS) -fPIC -shared -o $@ $(SRC)

$(StaticLib): hashmap.c hashmap.h
	@if [ ! -d lib ]; then mkdir lib; fi
	$(CC) -c $(CFLAGS) -o hashmap.o hashmap.c
	$(AR) rcs $@ hashmap.o
	@rm hashmap.o

Binaries=$(Test) $(Example) $(Benchmark) $(ProfileBin)

clean: py-clean
	@for file in $(Binaries) preproc.i $(ProfileFiles); do\
		if [ -f $$file ]; then\
			rm $$file;\
		fi;\
	done
	@if [ -d lib ]; then rm -rf lib; fi
	@for f in `find . -name '*.o'`; do rm $$f; done

proc:
	gcc -I. -E hashmap.c $(Test).c > preproc.i

ProfileFlags=-Wall -I. -std=c99 -pg
ProfileBin=profile.bin
Profiles=$(Benchmark)_prof $(Test)_prof
TestProfile=test
ProfileFiles=$(ProfileBin) gmon.out $(Profiles)

profile: hashmap.c tests/benchmarks.c tests/test.c tests/test_common.c
	@for f in hashmap tests/test_common; do\
		$(CC) $(ProfileFlags) -c -o $$f.o $$f.c; done

	@for file in $(Benchmark) $(Test); do\
		$(CC) $(ProfileFlags) -o "$$file".bin hashmap.o tests/test_common.o "$$file".c;\
		./"$$file".bin;\
		gprof -b ./"$$file".bin gmon.out > "$$file"_prof;\
		rm "$$file".bin gmon.out;\
	done
	@for f in `find . -name '*.o'`; do rm $$f; done

.PHONY: clean proc profile

PyTestDir=tests/python
include $(PyTestDir)/Makefile.in