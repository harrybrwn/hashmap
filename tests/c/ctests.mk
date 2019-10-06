Test=$(CTestDir)/test
Example=$(CTestDir)/doc_example
Benchmark=$(CTestDir)/benchmarks
InternalTest=$(CTestDir)/internal
LibTest=$(CTestDir)/test_lib
IterTest=$(CTestDir)/iter_test

AllCTests=$(Test) $(Example) $(Benchmark) $(InternalTest) $(LibTest) $(IterTest)

LoadSharedLib = LD_LIBRARY_PATH=./lib:$$LD_LIBRARY_PATH

.PHONY: c-test
c-test: $(StaticLib) $(AllCTests)
	@./$(Test)
	@./$(InternalTest)
	@./$(IterTest)
	@./$(Example) > /dev/null
	@$(LoadSharedLib) $(LibTest) > /dev/null

clean: clean-ctests
clean-ctests:
	$(RM) *.gcov $(AllCTests)

TestFlags=-I. -Isrc -Iinc -Wall -Wextra -Llib -g -lm -lshashmap

tests/c/%: tests/c/%.c $(UTEST) $(HASHMAP) $(StaticLib)
	$(CC) -o $@ $(UTEST) $< $(TestFlags)

$(Benchmark): $(Benchmark).c
	$(CC) -DTRASH_KEY -o $@ -O3 $^ $(TestFlags)

$(LibTest): $(LibTest).c $(SharedLib)
	$(CC) $(CFLAGS) $< -o $@ -lhashmap

mem-%: tests/c/%
	@valgrind --leak-check=full $<

test-cov: $(Test).c $(TestDir)/utest.c
	$(CC) $(CFLAGS) -fprofile-arcs -ftest-coverage $^ -o $(Test) -lm
	@$(Test) > /dev/null
	gcov test hashmap
	@$(RM) *.gcda *.gcno

internal-cov: $(InternalTest).c $(TestDir)/utest.c
	$(CC) $(CFLAGS) -fprofile-arcs -ftest-coverage $^ -o $(InternalTest) -lm
	@$(InternalTest) > /dev/null
	gcov internal hashmap
	@$(RM) *.gcda *.gcno

.PHONY: prof
prof: bench_prof.txt test_prof.txt

bench_prof.txt: $(Benchmark).c
	$(CC) -pg -Wall -Wextra -I. -Isrc $^ -o profile.bin
	@./profile.bin
	@if [ ! -f 'gmon.out' ]; then exit 1; fi
	gprof profile.bin gmon.out --no-time=randstring > $@
	@rm profile.bin

test_prof.txt: $(Test).c tests/utest.c
	$(CC) -pg -Wall -Wextra -I. -Isrc $^ -o profile.bin -lm
	@./profile.bin
	@if [ ! -f 'gmon.out' ]; then exit 1; fi
	gprof profile.bin gmon.out > $@
	@rm profile.bin