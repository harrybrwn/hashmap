CC=gcc
CFLAGS=-Wall -Wextra -L$(LibDir) -I. -Ofast
AR=ar

SRC=hashmap.c

# Directories
TestDir=tests
LibDir=lib
ExtDir=extentions
PyTestDir=$(TestDir)/python
CTestDir=$(TestDir)/c
CppTestDir=$(TestDir)/cpp

StaticLib=$(LibDir)/libhashmapstatic.a
SharedLib=$(LibDir)/libhashmap.so

TestCommon=$(TestDir)/test_common.o

all: lib build-test

include $(CTestDir)/Makefile
include $(PyTestDir)/Makefile
include $(CppTestDir)/Makefile

test: c-test cpp-test py-test

build-test: $(AllCTests) $(PyLib) $(AllCppTests)

bench: $(Benchmark)
	@./$(Benchmark)

.PHONY: all test bench

hashmap.o: hashmap.c hashmap.h
	$(CC) $(CFLAGS) -c $< -o $@

.PHONY: lib
lib: $(SharedLib) $(StaticLib)

$(SharedLib): $(SRC) hashmap.h
	@if [ ! -d lib ]; then mkdir lib; fi
	$(CC) $(CFLAGS) -fPIC -shared -o $@ $<

$(StaticLib): hashmap.o
	@if [ ! -d lib ]; then mkdir lib; fi
	$(AR) rcs $@ hashmap.o

$(TestCommon): $(TestDir)/test_common.c $(TestDir)/test_common.h
	$(CC) $(CFLAGS) -c $< -o $@

Binaries=$(Test) $(Example) $(Benchmark) $(InternalTest)

clean:
	@rm -f $(Binaries) gmon.out *_prof.txt profile.bin
	@rm -f $(shell find . -name '*.o')
	@rm -f $(shell find . -name '*.so')
	@rm -rf lib extentions/py/build tests/python/temp.* tests/cpp/test

prof: bench_prof.txt test_prof.txt

bench_prof.txt: hashmap.c $(Benchmark).c $(TestCommon:%.o=%.c)
	$(CC) -pg -Wall -Wextra -I. $^ -o profile.bin
	@./profile.bin
	@if [ ! -f 'gmon.out' ]; then exit 1; fi
	gprof profile.bin gmon.out > $@
	@rm profile.bin

test_prof.txt: $(Test).c $(TestCommon:%.o=%.c)
	$(CC) -pg -Wall -Wextra -I. $^ -o profile.bin
	@./profile.bin
	@if [ ! -f 'gmon.out' ]; then exit 1; fi
	gprof profile.bin gmon.out > $@
	@rm profile.bin

.PHONY: clean proc prof fmt
