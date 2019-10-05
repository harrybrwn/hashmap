Test=$(CTestDir)/test
Example=$(CTestDir)/doc_example
Benchmark=$(CTestDir)/benchmarks
InternalTest=$(CTestDir)/internal
LibTest=$(CTestDir)/test_lib
IterTest=$(CTestDir)/iter_test

AllCTests=$(Test) $(Example) $(Benchmark) $(InternalTest) $(LibTest) $(IterTest)

LoadSharedLib = LD_LIBRARY_PATH=./lib:$$LD_LIBRARY_PATH

.PHONY: c-test
c-test: $(AllCTests)
	@./$(Test)
	@./$(InternalTest)
	@./$(IterTest)
	@./$(Example) > /dev/null
	@$(LoadSharedLib) $(LibTest) > /dev/null

clean: clean-ctests
clean-ctests:
	$(RM) *.gcov $(AllCTests)

$(Test): $(HASHMAP) $(Test).c tests/test.h $(UTEST) tests/c/map_test.c
	$(CC) -Isrc $(CFLAGS) -o $@ $(Test).c $(UTEST) -lm

$(Example): $(StaticLib) $(Example).c
	$(CC) -o $@ $(CFLAGS) $@.c -lshashmap

$(Benchmark): $(Benchmark).c
	$(CC) -Isrc -DTRASH_KEY -o $@ $(CFLAGS) -O3 $^ -Wno-unused-parameter

$(InternalTest): $(HASHMAP) $(UTEST) $(InternalTest).c tests/test.h
	$(CC) -Isrc $(CFLAGS) -o $@ $(InternalTest).c $(UTEST) -lm

$(IterTest): $(IterTest).c hashmap.o $(UTEST)
	$(CC) -Isrc $(CFLAGS) $^ -o $@

$(LibTest): $(LibTest).c $(SharedLib)
	$(CC) $(CFLAGS) $< -o $@ -lhashmap

mem/%: tests/c/%
	@valgrind $<

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