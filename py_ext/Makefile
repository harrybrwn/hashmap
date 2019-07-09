CC=gcc
CFLAGS=-Wall -g -pthread -DNDEBUG -fwrapv -03 -fPIC -I$(Include)

Include=/usr/local/include/python3.7m
Shared=libhashmap.so

build: hashmap.o
	@# python setup.py build -cmingw32
	@# python setup.py build_ext --inplace
	python setup.py build

$(Shared): ../hashmap.c
	$(CC) $(CFLAGS) -c -I.. ../hashmap.o -o $@

.PHONY: build

hashmap.o: ../hashmap.c
	$(CC) -Wall -I.. -c -fPIC -o $@ ../hashmap.c

clean:
	rm -rf build hashmap.o hashmap *.so py_hashmap *.linux*

.PHONY: clean