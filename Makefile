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

install: lib singleheader
	sudo cp -f lib/libhashmap.so $(LibInstallDir)
	sudo cp -f lib/libshashmap.a $(LibInstallDir)
	sudo cp -f inc/hashmap.h $(HeaderInstallDir)

uninstall:
	sudo rm -f $(LibInstallDir)/libhashmap.so
	sudo rm -f $(LibInstallDir)/libshashmap.a
	sudo rm -f $(HeaderInstallDir)/hashmap.h
	sudo rm -f $(HeaderInstallDir)/map_iter.h

HASHMAP=src/hashmap.c

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

.PHONY: lib
lib: $(SharedLib) $(StaticLib)

LibBuildFlags = -Iinc -Wall -Wextra $(LibBuildMacros:%=-D%) -c

ifeq ($(BuildType),test)
LibBuildFlags+=-g
else
LibBuildFlags+=-O3
endif

lib/static/%.o: src/%.c inc/%.h lib/static
	$(CC) $(LibBuildFlags) $(filter %.c, $^) -o $@

lib/shared/%.o: src/%.c inc/%.h lib/shared
	$(CC) $(LibBuildFlags) -fPIC $(filter %.c, $^) -o $@

lib/static lib/shared:
	[ ! -d lib ] && mkdir lib/static lib/shared -p

$(SharedLib): lib/shared/hashmap.o
	$(CC) -shared $^ -o $@

$(StaticLib): lib/static/hashmap.o
	$(AR) rcs $@ $^

clean:
	rm -f gmon.out *_prof.txt profile.bin
	rm -f $(shell find . -name '*.o')
	rm -f $(shell find . -name '*.so')
	rm -rf lib extentions/py/build tests/python/temp.* tests/cpp/test

.PHONY: clean