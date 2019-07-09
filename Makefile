CC=gcc
CFLAGS=-Wall -L$(LibDir) -I. -std=c89 -g
AR=ar

SRC=hashmap.c

# Directories
TestDir=tests
LibDir=lib
ExtDir=extentions
PyTestDir=$(TestDir)/python
CTestDir=$(TestDir)/c
CppTestDir=$(TestDir)/cpp

# Binaries
Test=$(CTestDir)/test
Example=$(CTestDir)/doc_example
Benchmark=$(CTestDir)/benchmarks
TestSRC=$(TestDir)/test_common.c

StaticLib=$(LibDir)/libhashmapstatic.a
SharedLib=$(LibDir)/libhashmap.so

all: $(Test) $(Example) $(Benchmark) lib

test: py-test cpp-test $(Test) $(Example) $(Benchmark)
	@./$(Test)
	@./$(Example) > /dev/null

bench: $(Benchmark)
	@./$(Benchmark)

.PHONY: all test bench

hashmap.o: hashmap.c hashmap.h
	$(CC) $(CFLAGS) -c $< -o $@

$(Test): $(TestSRC) $(SRC) $(Test).c
	$(CC) -DHASHMAP_TESTING $(CFLAGS) -o $@ $^

$(Example): $(TestSRC) $(Example).c $(StaticLib)
	$(CC) -o $@ $(CFLAGS) $(TestSRC) $@.c -lhashmapstatic

$(Benchmark): $(TestSRC) $(StaticLib) $(Benchmark).c
	$(CC) -DHASHMAP_TESTING -o $@ $(CFLAGS) $(TestSRC) $@.c -lhashmapstatic

lib: $(SharedLib) $(StaticLib)
.PHONY: lib

$(SharedLib): $(SRC) hashmap.h
	@if [ ! -d lib ]; then mkdir lib; fi
	$(CC) $(CFLAGS) -fPIC -shared -o $@ $<

$(StaticLib): hashmap.o
	@if [ ! -d lib ]; then mkdir lib; fi
	$(AR) rcs $@ hashmap.o

Binaries=$(Test) $(Example) $(Benchmark) $(ProfileBin)

clean: py-clean
	@for file in $(Binaries) preproc.i $(ProfileFiles); do\
		if [ -f $$file ]; then\
			rm $$file;\
		fi;\
	done
	@if [ -d lib ]; then rm -rf lib; fi
	@for f in `find . -name '*.o'`; do rm $$f; done
	@if [ -f hashmap.o ]; then rm hashmap.o; fi

proc:
	gcc -I. -E hashmap.c $(Test).c > preproc.i

ProfileFlags=-Wall -I. -std=c99 -pg -DHASHMAP_TESTING
ProfileBin=profile.bin
Profiles=$(Benchmark)_prof $(Test)_prof
TestProfile=test
ProfileFiles=$(ProfileBin) gmon.out $(Profiles)

profile: hashmap.c hashmap.h tests/benchmarks.c tests/test.c tests/test_common.c
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

include $(PyTestDir)/Makefile.in
include $(CppTestDir)/Makefile.in