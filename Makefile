CC=gcc
CFLAGS=-Wall -Wextra -Llib -Iinc -I. -g
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
	@echo "  install:   copy header files, static and shared libraries to '$(HeaderInstallDir)' and '$(LibInstallDir)'"
	@echo "  uninstall: remove header files, static and shared libraries from '$(HeaderInstallDir)' and '$(InstallDir)'"
	@echo "  test:      run all tests"
	@echo "  clean:     remove extra files from build"
	@echo "  bench:     run the benchmarks"
	@echo "  lib:       build only the static and shared libraries (found in ./lib)"

all: lib build-tests

install: lib
	sudo cp -f lib/libhashmap.so $(LibInstallDir)
	sudo cp -f lib/libshashmap.a $(LibInstallDir)
	sudo cp -f inc/hashmap.h $(HeaderInstallDir)
	sudo cp -f inc/map_iter.h $(HeaderInstallDir)

uninstall:
	sudo rm -f $(LibInstallDir)/libhashmap.so
	sudo rm -f $(LibInstallDir)/libshashmap.a
	sudo rm -f $(HeaderInstallDir)/hashmap.h
	sudo rm -f $(HeaderInstallDir)/map_iter.h

HASHMAP=src/hashmap.c
MAPITER=src/map_iter.c

include $(CTestDir)/ctests.mk
include $(CppTestDir)/cpptests.mk
include $(PyTestDir)/pytests.mk

test: c-test cpp-test py-test

build-tests: $(AllCTests) $(PyLib) $(AllCppTests)

bench: $(Benchmark)
	@./$(Benchmark)

.PHONY: all test bench build-tests

%.o: src/%.c inc/%.h
	$(CC) $(CFLAGS) -c $(filter %.c, $^) -o $(notdir $@)

.PHONY: lib lib-setup
lib: lib-setup $(SharedLib) $(StaticLib)

lib-setup:
	@if [ ! -d lib ]; then mkdir lib; fi

LibBuildFlags += -Iinc -Wall -Wextra -O3 -c

lib/%_static.o: lib-setup src/%.c inc/%.h
	$(CC) $(LibBuildFlags) $(filter %.c, $^) -o $@

lib/%_shared.o: lib-setup src/%.c inc/%.h
	$(CC) $(LibBuildFlags) -fPIC $(filter %.c, $^) -o $@

$(SharedLib): lib/hashmap_shared.o lib/map_iter_shared.o
	$(CC) -shared $^ -o $@

$(StaticLib): lib/hashmap_static.o lib/map_iter_static.o
	$(AR) rcs $@ $^

clean:
	rm -f gmon.out *_prof.txt profile.bin
	rm -f $(shell find . -name '*.o')
	rm -f $(shell find . -name '*.so')
	rm -rf lib extentions/py/build tests/python/temp.* tests/cpp/test

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