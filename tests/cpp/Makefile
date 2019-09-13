CPPFLAGS=-Wall -Wextra -Werror -L./lib -I. -g
CPP_FILE=$(ExtDir)/cpp/HashMap
CPP_SRC=$(CPP_FILE).cpp $(CPP_FILE).hpp

AllCppTests=$(CppTest)

CppTest=$(TestDir)/cpp/test

.PHONY: cpp-test
cpp-test: $(CppTest)
	@./$(CppTest)

$(CppTest): $(CPP_FILE).o hashmap.o $(CppTest).cpp
	$(CC) $(CPPFLAGS) $^ -o $@ -lstdc++

$(CPP_FILE).o: $(CPP_FILE).cpp $(CPP_FILE).hpp
	$(CC) $(CPPFLAGS) -c $< -o $@ -lstdc++