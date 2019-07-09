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

Binaries=$(Test) $(Example) $(Benchmark) $(ProfileBin)

clean: py-clean cpp-clean
	@for file in $(Binaries) preproc.i $(ProfileFiles); do\
		if [ -f $$file ]; then\
			rm $$file;\
		fi;\
	done
	@if [ -d lib ]; then rm -rf lib; fi
	@if [ -d build ]; then rm -rf build; fi
	@for f in `find . -name '*.o'`; do rm $$f; done
	@if [ -f hashmap.o ]; then rm hashmap.o; fi

proc:
	gcc -I. -E hashmap.c $(Test).c > preproc.i

ProfileFlags=-Wall -I. -std=c99 -pg -DHASHMAP_TESTING
ProfileBin=profile.bin
Profiles=$(Benchmark)_prof $(Test)_prof
TestProfile=test
ProfileFiles=$(ProfileBin) gmon.out $(Profiles)

profile: hashmap.c hashmap.h $(Benchmark).c $(Test).c $(TestCommon:%.o=%.c)
	@for f in hashmap tests/test_common; do\
		$(CC) $(ProfileFlags) -c -o $$f.o $$f.c; done

	@for file in $(Benchmark) $(Test); do\
		$(CC) $(ProfileFlags) -o "$$file".bin hashmap.o tests/test_common.o "$$file".c;\
		./"$$file".bin;\
		gprof -b ./"$$file".bin gmon.out > "$$file"_prof;\
		rm "$$file".bin gmon.out;\
	done
	@# for f in `find . -name '*.o'`; do rm $$f; done

.PHONY: clean proc profile
