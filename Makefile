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

hashmap.o: hashmap.c hashmap.h internal/_hashmap.c internal/_hashmap.h
	$(CC) $(CFLAGS) -c $< -o _$@
	$(CC) $(CFLAGS) -c internal/_hashmap.c -o internal/_hashmap.o
	ld -relocatable _$@ internal/_hashmap.o -o $@
	@rm _$@ internal/_hashmap.o

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

Binaries=$(Test) $(Example) $(Benchmark) $(ProfileBin) $(InternalTest)

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

ProfileFlags=-Wall -I. -pg -DHASHMAP_TESTING
ProfileBin=profile.bin
Profiles=$(Benchmark)_prof $(Test)_prof
TestProfile=test
ProfileFiles=$(ProfileBin) gmon.out $(Profiles)

profile: hashmap.c internal/_hashmap.c $(Benchmark).c $(Test).c $(TestCommon:%.o=%.c)
	@for f in hashmap internal/_hashmap tests/test_common; do\
		$(CC) $(ProfileFlags) -c -o $$f.o $$f.c; done

	@for file in $(Benchmark) $(Test); do\
		$(CC) $(ProfileFlags) -o "$$file".bin hashmap.o internal/_hashmap.o tests/test_common.o "$$file".c;\
		./"$$file".bin;\
		gprof -b ./"$$file".bin gmon.out > "$$file"_prof;\
		rm "$$file".bin gmon.out;\
	done
	@# for f in `find . -name '*.o'`; do rm $$f; done

.PHONY: clean proc profile
