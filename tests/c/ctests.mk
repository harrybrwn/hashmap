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
	@./$(Example) > /dev/null
	@./$(InternalTest)
	@$(LoadSharedLib) $(LibTest) > /dev/null
	@./$(IterTest)

clean: clean-ctests
clean-ctests:
	@$(RM) $(LibTest) *.gcov $(CTestDir)/iter_test

$(Test): hashmap.c $(Test).c tests/test.h tests/utest.o
	$(CC) -DHASHMAP_TESTING $(CFLAGS) -o $@ $(Test).c tests/utest.o -lm

$(Example): $(StaticLib) $(Example).c
	$(CC) -o $@ $(CFLAGS) $@.c -lshashmap

$(Benchmark): $(Benchmark).c
	$(CC) -DTRASH_KEY -o $@ $(CFLAGS) -O3 $^ -Wno-unused-parameter

$(InternalTest): hashmap.c tests/utest.o $(InternalTest).c tests/test.h
	$(CC) $(CFLAGS) -o $@ $(InternalTest).c tests/utest.o -lm

$(IterTest): $(IterTest).c hashmap.o $(UTEST)
	$(CC) $(CFLAGS) $^ -o $@

.PHONY: lib-test
lib-test: $(LibTest)
	@$(LoadSharedLib) valgrind $(LibTest)
	@#$(LoadSharedLib) $(LibTest)

$(LibTest): $(LibTest).c $(SharedLib)
	$(CC) $(CFLAGS) $< -o $@ -lhashmap

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