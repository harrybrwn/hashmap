CPPFLAGS=-Wall -Wextra -Werror -L$(LibDir) -I. -g -DHASHMAP_TESTING
CPP_FILE=$(ExtDir)/cpp/HashMap
CPP_SRC=$(CPP_FILE).cpp $(CPP_FILE).hpp

AllCppTests=$(CppTest)

CppTest=$(TestDir)/cpp/test

.PHONY: cpp-test cpp-clean
cpp-test: $(CppTest)
	@./$(CppTest)

cpp-clean:
	@if [ -f $(CppTest) ]; then rm $(CppTest); fi

$(CppTest): $(CPP_FILE).o $(ExtDir)/cpp/chashmap.o $(CppTest).cpp $(TestCommon)
	$(CC) $(CPPFLAGS) $^ -o $@ -lstdc++

$(CPP_FILE).o: $(CPP_FILE).cpp $(CPP_FILE).hpp
	$(CC) $(CPPFLAGS) -c $< -o $@ -lstdc++

$(ExtDir)/cpp/chashmap.o: hashmap.c hashmap.h
	$(CC) $(CPPFLAGS) -c $< -o $@ -lstdc++

$(ExtDir)/cpp/test_common.o: $(TestDir)/test_common.c $(TestDir)/test_common.h
	$(CC) $(CPPFLAGS) -c $< -o $@
