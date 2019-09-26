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
	@$(RM) $(LibTest)

clean: clean-ctests

$(Test): hashmap.c $(Test).c tests/test.h
	$(CC) -DHASHMAP_TESTING $(CFLAGS) -o $@ $(Test).c

$(Example): $(StaticLib) $(Example).c
	$(CC) -o $@ $(CFLAGS) $@.c -lhashmapstatic

$(Benchmark): $(Benchmark).c
	$(CC) -DTRASH_KEY -o $@ $(CFLAGS) -O3 $^ -Wno-unused-parameter

$(InternalTest): hashmap.c $(InternalTest).c tests/test.h
	$(CC) $(CFLAGS) -o $@ $(InternalTest).c

.PHONY: lib-test
lib-test: $(LibTest)
	$(LoadLib) valgrind $(LibTest)

$(LibTest): $(LibTest).c $(SharedLib)
	@#$(CC) $(CFLAGS) -libhashmap.so $< -o $@
	$(CC) $(CFLAGS) $< -o $@ -lhashmap