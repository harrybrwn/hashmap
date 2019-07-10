AllCTests=$(Test) $(Example) $(Benchmark)

Test=$(CTestDir)/test
Example=$(CTestDir)/doc_example
Benchmark=$(CTestDir)/benchmarks

.PHONY: c-test
c-test: $(Test) $(Example)
	@./$(Test)
	@./$(Example) > /dev/null

$(Test): $(TestCommon) $(SRC) $(Test).c
	$(CC) -DHASHMAP_TESTING $(CFLAGS) -o $@ $^

$(Example): $(TestCommon) $(StaticLib) $(Example).c
	$(CC) -o $@ $(CFLAGS) $(TestSRC) $@.c -lhashmapstatic

$(Benchmark): $(TestCommon) hashmap.o $(Benchmark).c
	$(CC) -o $@ $(CFLAGS) $^
