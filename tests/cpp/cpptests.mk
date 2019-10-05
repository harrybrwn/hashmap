CPPFLAGS=-Wall -Wextra -Werror -Llib -I. -g
CPP_FILE=$(ExtDir)/cpp/HashMap
CPP_SRC=$(CPP_FILE).cpp $(CPP_FILE).hpp

AllCppTests=$(CppTest)

CppTest=$(TestDir)/cpp/test

.PHONY: cpp-test
cpp-test: $(CppTest)
	@./$(CppTest)

$(CppTest): $(CPP_FILE).o $(CppTest).cpp
	$(CC) $(CPPFLAGS) $^ -o $@ -lstdc++ -lshashmap

$(CPP_FILE).o: $(CPP_FILE).cpp $(CPP_FILE).hpp
	$(CC) $(CPPFLAGS) -c $< -o $@ -lstdc++