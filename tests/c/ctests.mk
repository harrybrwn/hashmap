Test=$(CTestDir)/test
Example=$(CTestDir)/doc_example
Benchmark=$(CTestDir)/benchmarks
InternalTest=$(CTestDir)/internal
LibTest=$(CTestDir)/test_lib

AllCTests=$(Test) $(Example) $(Benchmark) $(InternalTest) $(LibTest)

LoadLib = LD_LIBRARY_PATH=./lib:$$LD_LIBRARY_PATH

.PHONY: c-test
c-test: $(AllCTests)
	@./$(Test)
	@./$(Example) > /dev/null
	@./$(InternalTest)
	@$(LoadLib) $(LibTest) > /dev/null

clean-ctests:
	@$(RM) $(LibTest) *.gcov

clean: clean-ctests

$(Test): hashmap.c $(Test).c tests/test.h tests/utest.o
	$(CC) -DHASHMAP_TESTING $(CFLAGS) -o $@ $(Test).c tests/utest.o -lm

$(Example): $(StaticLib) $(Example).c
	$(CC) -o $@ $(CFLAGS) $@.c -lhashmapstatic

$(Benchmark): $(Benchmark).c
	$(CC) -DTRASH_KEY -o $@ $(CFLAGS) -O3 $^ -Wno-unused-parameter

$(InternalTest): hashmap.c tests/utest.o $(InternalTest).c tests/test.h
	$(CC) $(CFLAGS) -o $@ $(InternalTest).c tests/utest.o -lm

$(TestDir)/utest.o: $(TestDir)/utest.c
	$(CC) $(CFLAGS) -c $< -o $@

.PHONY: lib-test
lib-test: $(LibTest)
	$(LoadLib) valgrind $(LibTest)

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