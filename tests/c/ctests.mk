AllCTests=$(Test) $(Example) $(Benchmark) $(InternalTest)

Test=$(CTestDir)/test
Example=$(CTestDir)/doc_example
Benchmark=$(CTestDir)/benchmarks
InternalTest=$(CTestDir)/internal
# LibTest=$(CTestDir)/test_lib

.PHONY: c-test
c-test: $(Test) $(Example) $(InternalTest)
	@./$(Test)
	@./$(Example) > /dev/null
	@./$(InternalTest)

$(Test): hashmap.c $(Test).c tests/test.h
	$(CC) -DHASHMAP_TESTING $(CFLAGS) -o $@ $(Test).c

$(Example): $(StaticLib) $(Example).c
	$(CC) -o $@ $(CFLAGS) $@.c -lhashmapstatic

$(Benchmark): $(Benchmark).c
	$(CC) -DTRASH_KEY -o $@ $(CFLAGS) -O3 $^ -Wno-unused-parameter

$(InternalTest): hashmap.c $(InternalTest).c tests/test.h
	$(CC) $(CFLAGS) -o $@ $(InternalTest).c

# $(LibTest): lib