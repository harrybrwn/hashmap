AllCTests=$(Test) $(Example) $(Benchmark)

Test=$(CTestDir)/test
Example=$(CTestDir)/doc_example
Benchmark=$(CTestDir)/benchmarks
InternalTest=$(CTestDir)/internal

.PHONY: c-test
c-test: $(Test) $(Example) $(InternalTest)
	@./$(Test)
	@./$(Example) > /dev/null
	@./$(InternalTest)

$(Test): $(TestCommon) $(SRC) internal/_hashmap.c $(Test).c
	$(CC) -DHASHMAP_TESTING $(CFLAGS) -o $@ $^

$(Example): $(TestCommon) $(StaticLib) $(Example).c
	$(CC) -o $@ $(CFLAGS) $(TestSRC) $@.c -lhashmapstatic

$(Benchmark): $(TestCommon) hashmap.c internal/_hashmap.c $(Benchmark).c
	$(CC) -DTRASH_KEY -o $@ $(CFLAGS) $^

$(InternalTest): $(TestCommon) hashmap.o $(InternalTest).c
	$(CC) $(CFLAGS) $^ -o $@