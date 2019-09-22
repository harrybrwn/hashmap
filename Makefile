CC=gcc
CFLAGS=-Wall -Wextra -Llib -I.
AR=ar

SRC=hashmap.c

# Directories
TestDir=tests
ExtDir=extentions
PyTestDir=$(TestDir)/python
CTestDir=$(TestDir)/c
CppTestDir=$(TestDir)/cpp

StaticLib=lib/libhashmapstatic.a
SharedLib=lib/libhashmap.so

all: lib build-tests

include $(CTestDir)/ctests.mk
include $(CppTestDir)/cpptests.mk
include $(PyTestDir)/pytests.mk

test: c-test cpp-test py-test

build-tests: $(AllCTests) $(PyLib) $(AllCppTests)

bench: $(Benchmark)
	@./$(Benchmark)

.PHONY: all test bench build-tests

hashmap.o: hashmap.c hashmap.h
	$(CC) $(CFLAGS) -c $< -o $@

.PHONY: lib
lib: $(SharedLib) $(StaticLib)

$(SharedLib): hashmap.c hashmap.h
	@if [ ! -d lib ]; then mkdir lib; fi
	$(CC) $(CFLAGS) -fPIC -shared -o $@ $<

$(StaticLib): hashmap.o
	@if [ ! -d lib ]; then mkdir lib; fi
	$(AR) rcs $@ hashmap.o

Binaries=$(Test) $(Example) $(Benchmark) $(InternalTest)

clean:
	@rm -f $(Binaries) gmon.out *_prof.txt profile.bin
	@rm -f $(shell find . -name '*.o')
	@rm -f $(shell find . -name '*.so')
	@rm -rf lib extentions/py/build tests/python/temp.* tests/cpp/test

prof: bench_prof.txt test_prof.txt

bench_prof.txt: $(Benchmark).c
	$(CC) -pg -Wall -Wextra -I. $^ -o profile.bin
	@./profile.bin
	@if [ ! -f 'gmon.out' ]; then exit 1; fi
	gprof profile.bin gmon.out --no-time=randstring > $@
	@rm profile.bin

test_prof.txt: $(Test).c
	$(CC) -pg -Wall -Wextra -I. $^ -o profile.bin
	@./profile.bin
	@if [ ! -f 'gmon.out' ]; then exit 1; fi
	gprof profile.bin gmon.out > $@
	@rm profile.bin

.PHONY: clean proc prof fmt