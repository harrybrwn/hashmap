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

StaticLib=lib/libshashmap.a
SharedLib=lib/libhashmap.so

include config.mk

UTEST=$(TestDir)/utest.o

h: help
help:
	@echo "  help:      desciptions of the build targets"
	@echo "  all:       compile everything"
	@echo "  install:   build static and shared libraries and move them to '$(InstallDir)'"
	@echo "  uninstall: remove static and shared libraries from '$(InstallDir)'"
	@echo "  test:      run all tests"
	@echo "  clean:     remove extra files from build"
	@echo "  bench:     run the benchmarks"
	@echo "  lib:       build only the static and shared libraries (found in ./lib)"

all: lib build-tests

install: lib
	@echo "install doesn't do anything yet"

uninstall:
	@echo "uninstall doesn't do anything yet"

include $(CTestDir)/ctests.mk
include $(CppTestDir)/cpptests.mk
include $(PyTestDir)/pytests.mk

test: c-test cpp-test py-test

build-tests: $(AllCTests) $(PyLib) $(AllCppTests)

bench: $(Benchmark)
	@./$(Benchmark)

.PHONY: all test bench build-tests

%.o: %.c %.h
	$(CC) $(CFLAGS) -c $(filter %.c, $^) -o $@

.PHONY: lib
lib: $(SharedLib) $(StaticLib)

$(SharedLib): hashmap.c hashmap.h map_iter.c map_iter.h
	@if [ ! -d lib ]; then mkdir lib; fi
	$(CC) $(CFLAGS) -fPIC -shared -o $@ $(filter %.c, $^)

$(StaticLib): hashmap.o map_iter.o
	@if [ ! -d lib ]; then mkdir lib; fi
	$(AR) rcs $@ $^

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

test_prof.txt: $(Test).c tests/utest.c
	$(CC) -pg -Wall -Wextra -I. $^ -o profile.bin
	@./profile.bin
	@if [ ! -f 'gmon.out' ]; then exit 1; fi
	gprof profile.bin gmon.out > $@
	@rm profile.bin

.PHONY: clean proc prof fmt